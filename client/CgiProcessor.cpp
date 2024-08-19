#include "CgiProcessor.hpp"

CgiProcessor::CgiProcessor(Request &request_, ServerConfigData *serverConfig_, LocationConfigData *locationConfig_)
	:request(request_)
	, serverConfig(serverConfig_)
	, locationConfig(locationConfig_)
{
}

CgiProcessor::~CgiProcessor()
{

}

CgiProcessor::CgiProcessor(const CgiProcessor& rhs)
	:request(rhs.request)
	, serverConfig(rhs.serverConfig)
{
}

CgiProcessor& CgiProcessor::operator=(const CgiProcessor& rhs)
{
	request = rhs.request;
	serverConfig = rhs.serverConfig;
	return (*this);
}

std::string	CgiProcessor::getScriptFile()
{
	return (scriptFile);
}

void	CgiProcessor::setURLEnv()
{
	metaVariables.push_back("SERVER_NAME=" + request.header["host"].front());
	metaVariables.push_back("SERVER_PORT=" + to_string(request.port));
	metaVariables.push_back("PATH_INFO=" + request.url);
	metaVariables.push_back("SCRIPT_NAME=" + scriptFile);
	std::deque<std::string>	queryStrings;
	std::string				metaQueryString;
	for (std::map<std::string, std::string>::iterator iter=request.query.begin(); iter != request.query.end(); iter++)
	{
		std::string key = iter->first;
		std::string value = iter->second;
		queryStrings.push_back(key.append("=").append(value));
	}
	for (size_t idx = 0; idx < queryStrings.size(); idx++)
	{
		metaQueryString += queryStrings[idx];
		if (idx == queryStrings.size() - 1)
			continue;
		metaQueryString = metaQueryString.append("&");
	}
	metaVariables.push_back("QUERY_STRING=" + metaQueryString);
}

bool	CgiProcessor::setStartHeaderEnv()
{
	metaVariables.push_back("SERVER_PROTOCOL=HTTP/1.1");
	std::string	method;
	if (request.method == GET)
		method = "GET";
	else if (request.method == POST)
		method = "POST";
	metaVariables.push_back("REQUEST_METHOD=" + method);
	if (request.header.find("Content-length") != request.header.end()
		&& request.header["Content-length"].size() > 0)
	{
		if (!atoi(request.header["Content-length"].front().c_str()))
		{
			request.status = 400;
			return (false);
		}
		metaVariables.push_back("CONTENT_LENGTH=" + request.header["Content-length"].front());
	}
	if (request.header.find("Content-type") != request.header.end()
		&& request.header["Content-type"].size() > 0)
	{
		metaVariables.push_back("CONTENT_TYPE=" + request.header["Content-type"].front());
	}
	if (request.header.find("Authorization") != request.header.end()
		&& request.header["Authorization"].size())
	{
		metaVariables.push_back("AUTH_TYPE=" + request.header["Authorization"][0]);
		if (request.header["Authorization"].size() <= 2)
			metaVariables.push_back("REMOTE_USER" + request.header["Authorization"][1]);
	}
	metaVariables.push_back("REMOTE_ADDR=" + request.clientIp);
	return (true);
}

bool	CgiProcessor::checkURL(std::string url)
{
	// Find CGI Script File in URL
	const std::string	availCgiExtensions[2] = {".py", ".php"};
	std::string			cgiExtension;
	size_t				cgiFilePos;
	for (int i=0; i<2; i++)
	{
		cgiExtension = availCgiExtensions[i];
		cgiFilePos = url.find(cgiExtension);
		if (cgiFilePos != std::string::npos)
			break ;
	}
	std::cout << "-------------------CGI TEST----------------------\n";
	std::cout << url << '\n';
	std::cout << locationConfig->getRoot() << '\n';
	for (auto iter = request.header["host"].begin(); iter != request.header["host"].end(); iter++)
		std::cout << *iter << ' ';
	std::cout << '\n';
	if (cgiFilePos == std::string::npos)
	{
		std::cout << "ㅋ 1" << std::endl;
		std::cout << "-------------------CGI END!----------------------\n";
		return (false);
	}
	scriptFile = url.substr(0, cgiFilePos + cgiExtension.size());
	size_t	parentDirReservedPos = url.find("..");
	if (parentDirReservedPos != std::string::npos)
	{
		std::cout << "ㅋㅋ 2" << std::endl;
		request.status = 404;
		std::cout << "-------------------CGI END!----------------------\n";
		return (false);
	}
	/* Check Available CGI Script Directories */
	// ebebebe

	/* Let's set the URL Environments Variables */
	setURLEnv();

	/* Let's set the StartLine Environments Variables and the Necessary HeaderLine Environments */
	setStartHeaderEnv();

	std::cout << "-------------------CGI END!----------------------\n";
	return (true);
}

void	CgiProcessor::executeCGIScript(const std::string path)
{
	int pipefd[2];
	if (pipe(pipefd) < 0)
	{
		request.status = 500;
		return ;
	}
	int pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		request.status = 500;
		return ;
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		std::string pythonCmd = "/usr/local/bin/python3";
		char *argv[] = {const_cast<char *>(&pythonCmd[0]), const_cast<char *>(&path[0]), NULL};
		char **envp = new char*[metaVariables.size() + 1];
		for (size_t i=0; i<metaVariables.size(); i++)
			envp[i] = const_cast<char *>(metaVariables[i].c_str());
		envp[metaVariables.size()] = 0;
		if (execve(&pythonCmd[0], argv, envp) == -1)
		{
			request.status = 500;
			exit(1);
		}
	}
	else
	{
		close(pipefd[1]);
		char buf[65537];
		int size;
		while (1)
		{
			size = read(pipefd[0], buf, 65536);
			if (size <= 0)
			{
				close(pipefd[0]);
				if (size < 0)
					request.status = 500;
				return ;
			}
			buf[size] = 0;
			cgiContent.append(std::string(buf));
		}
		close(pipefd[0]);
	}
}
