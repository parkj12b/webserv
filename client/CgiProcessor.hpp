#ifndef CGI_PROCESSOR_HPP
# define CGI_PROCESSOR_HPP
# include <map>
# include <deque>
# include "Response.hpp"
# include "ServerConfigData.hpp"

struct Request;

class CgiProcessor
{
public:
	CgiProcessor(Request &request_, ServerConfigData *serverConfig_, LocationConfigData *locationConfig_);
	~CgiProcessor();
	CgiProcessor(const CgiProcessor& rhs);
	CgiProcessor& operator=(const CgiProcessor& rhs);
	std::string		getScriptFile();
	bool	checkURL(std::string url);
	// void	setURLEnv(Request &request);
	// void	setRequestHeaderEnv(Method method, std::map<std::string, std::deque<std::string> >& headers);
	// void	executeCGIScript(std::string path);
private:
	CgiProcessor();
	std::map<std::string, std::deque<std::string> > cgiResponseHeader;
	Request 			&request;
	ServerConfigData	*serverConfig;
	LocationConfigData	*locationConfig;
	std::string			scriptFile;
};

#endif
