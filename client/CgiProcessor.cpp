#include "CgiProcessor.hpp"

CgiProcessor::CgiProcessor(Request &request_, ServerConfigData *serverConfig_)
	:request(request)
	, serverConfig(serverConfig_)
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
}