#include "CgiProcessor.hpp"
#include "Kq.hpp"

CgiProcessor::CgiProcessor(Request &request_, ServerConfigData *serverConfig_, LocationConfigData *locationConfig_)
	:request(request_)
	, serverConfig(serverConfig_)
	, locationConfig(locationConfig_)
	, contentLength(0)
	, fin(false)
{
}

CgiProcessor::~CgiProcessor()
{
}

string	CgiProcessor::getScriptFile()
{
	return (scriptFile);
}

string	CgiProcessor::getCgiContent()
{
	return (cgiContent);
}

size_t	CgiProcessor::getContentLength()
{
	return (contentLength);
}

bool	CgiProcessor::getFin()
{
	return (fin);
}

void	CgiProcessor::insertEnv(string key, string value)
{
	metaVariables.insert(make_pair(key, value));
}

void	CgiProcessor::setURLEnv()
{
	insertEnv("SERVER_NAME", request.header["host"].front());
	insertEnv("SERVER_PORT", to_string(request.port));
	insertEnv("PATH_INFO", request.url);
	insertEnv("SCRIPT_NAME", scriptFile);
	deque<string>	queryStrings;
	string			metaQueryString;
	for (map<string, string>::iterator iter=request.query.begin(); iter != request.query.end(); iter++)
	{
		string key = iter->first;
		string value = iter->second;
		queryStrings.push_back(key.append("=").append(value));
	}
	for (size_t idx = 0; idx < queryStrings.size(); idx++)
	{
		metaQueryString += queryStrings[idx];
		if (idx == queryStrings.size() - 1)
			continue;
		metaQueryString = metaQueryString.append("&");
	}
	insertEnv("QUERY_STRING", metaQueryString);
}

void	CgiProcessor::setStartHeaderEnv()
{
	insertEnv("SERVER_PROTOCOL", "HTTP/1.1");
	string	method;
	if (request.method == GET)
		method = "GET";
	else if (request.method == POST)
		method = "POST";
	insertEnv("REQUEST_METHOD", method);
	if (request.header.find("Content-length") != request.header.end()
		&& request.header["Content-length"].size() > 0)
	{
		insertEnv("CONTENT_LENGTH", request.header["Content-length"].front());
	}
	if (request.header.find("Content-type") != request.header.end()
		&& request.header["Content-type"].size() > 0)
	{
		insertEnv("CONTENT_TYPE", request.header["Content-type"].front());
	}
	if (request.header.find("Authorization") != request.header.end()
		&& request.header["Authorization"].size())
	{
		insertEnv("AUTH_TYPE", request.header["Authorization"][0]);
		if (request.header["Authorization"].size() <= 2)
			insertEnv("REMOTE_USER", request.header["Authorization"][1]);
	}
	insertEnv("REMOTE_ADDR", request.clientIp);
}

void	CgiProcessor::selectCgiCmd(string url)
{
	// Find CGI Script File in URL
	const string	availCgiExtensions[2] = {".py", ".php"};
	string			cgiExtension;
	size_t			cgiFilePos;
	for (int i=0; i<2; i++)
	{
		cgiExtension = availCgiExtensions[i];
		cgiFilePos = url.find(cgiExtension);
		if (cgiFilePos != string::npos)
			break ;
	}
	cgiCommand = (!cgiExtension.compare(".py")) ? "python3" : "php";
	scriptFile = url.substr(0, cgiFilePos + cgiExtension.size());
}

void	CgiProcessor::checkPostContentType()
{
	if (request.header.find("content-type") == request.header.end()
		|| request.header.find("content-length") == request.header.end()
		|| atol(request.header["content-length"].front().c_str()) <= 0
		|| atol(request.header["content-length"].front().c_str()) > locationConfig->getClientMaxBodySize())
	{
		request.status = 400;
		return ;
	}
	if (!request.header["content-type"].front().compare("application/x-www-form-urlencoded")
		|| !request.header["content-type"].front().compare("application/json"))
		executeCGIScript(scriptFile);
	else if (!request.header["content-type"].front().compare("multipart/form-data"))
	{
		scriptFile = "/upload/upload.py";
		executeCGIScript(scriptFile);
	}
	else
		request.status = 400;
}

void	CgiProcessor::executeCGIScript(const string path)
{
	scriptFile = path;
	setURLEnv();
	setStartHeaderEnv();
	int pipefd[2];
	if (pipe(pipefd) < 0)
	{
		request.status = 500;
		return ;
	}
	pid_t pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		request.status = 500;
		return ;
	}
	Kq::plusEvent(pipefd[0], EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
	if (pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		dup2(pipefd[1], STDERR_FILENO);
		close(pipefd[1]);
		char *argv[] = {const_cast<char *>(&cgiCommand[0]), const_cast<char *>(&path[0]), NULL};
		char **envp = new char*[metaVariables.size() + 1];
		size_t	idx = 0;
		for (map<string, string>::iterator iter = metaVariables.begin(); iter != metaVariables.end(); iter++)
		{
			string	env;
			env = env.append(iter->first).append("=").append(iter->second);
			envp[idx] = new char[env.size() + 1];
			strcpy(envp[idx++], env.c_str());
		}
		envp[metaVariables.size()] = 0;
		if (execve(&cgiCommand[0], argv, envp) == -1)
		{
			request.status = 500;
			exit(1);
		}
	}
	else
	{
		close(pipefd[1]);
		Kq::processor.push_back(pid);
		int tmpFd = pipefd[0];
		close(pipefd[0]);
		Kq::cgiFd.insert(make_pair(tmpFd, request.clientFd));
	}
}
