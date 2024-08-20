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
	CgiProcessor&	operator=(const CgiProcessor& rhs);
	std::string		getScriptFile();
	std::string		getCgiContent();
	bool			checkURL(std::string url);
	void			executeCGIScript(const std::string path);
	void			setMetaVariable(string key, string value);
private:
	CgiProcessor();
	void	setURLEnv();
	bool	setStartHeaderEnv();
	Request 			&request;
	ServerConfigData	*serverConfig;
	LocationConfigData	*locationConfig;
	std::string			scriptFile;
	std::string			cgiContent;
	std::deque<std::string>	metaVariables;
	std::map<std::string, std::deque<std::string> > cgiResponseHeader;
};

#endif
