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

bool	CgiProcessor::checkURL(std::string url)
{
	// Find CGI Script File in URL
	const std::string	availCgiExtensions[2] = {".py", "php"};
	std::string			cgiExtension;
	size_t				cgiFilePos;
	for (int i=0; i<2; i++)
	{
		cgiExtension = availCgiExtensions[i];
		cgiFilePos = url.find(cgiExtension);
		if (cgiFilePos != std::string::npos)
			break ;
	}
	if (cgiFilePos == std::string::npos)
	{
		std::cout << "병신" << std::endl;
		return (false);
	}
	scriptFile = url.substr(0, cgiFilePos + cgiExtension.size());
	size_t	parentDirReservedPos = url.find("..");
	if (parentDirReservedPos != std::string::npos)
	{
		std::cout << "병신" << std::endl;
		return (false);
	}
	/* Check Available CGI Script Directories */
	// ebebebe
	std::cout << locationConfig->getRoot() << '\n';

	/* Let's set the URL Environments Variables */
	

	/* Let's set the StartLine Environments Variables and the Necessary HeaderLine Environments */
	// ebssbss
	return (true);
}