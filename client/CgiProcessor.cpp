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

void	CgiProcessor::setStartHeaderEnv()
{
	metaVariables.push_back("SERVER_PROTOCOL=HTTP/1.1");
	std::string	method;
	if (request.method == GET)
		method = "GET";
	else if (request.method == POST)
		method = "POST";
	metaVariables.push_back("REQUEST_METHOD=" + method);
	if (request.header.find("Content-length") != request.header.end()
		&& request.header["Content-length"].size() > 0 && atoi(request.header["Content-length"].front().c_str()) > 0)
	{
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
	if (request.header.find("") != request.header.end()
		&& request.header[""].size())
	{
		metaVariables.push_back("REMOTE_ADDR=" + );
	}
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