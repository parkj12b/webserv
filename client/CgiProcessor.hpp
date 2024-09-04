#ifndef CGI_PROCESSOR_HPP
# define CGI_PROCESSOR_HPP
# include <map>
# include <deque>
# include "Response.hpp"
# include "ServerConfigData.hpp"
using namespace std;

struct Request;

class CgiProcessor
{
public:
	CgiProcessor(Request &request_, ServerConfigData *serverConfig_, LocationConfigData *locationConfig_, string pathEnv_);
	~CgiProcessor();
	string	getScriptFile();
	string	getCgiContent();
	size_t	getContentLength();
	bool	getFin();
	void	insertEnv(string key, string value);
	bool	isValidUploadPath();
	void	checkPostContentType(const string path);
	void	executeCGIScript(const string path);
	static  string EXECUTE_PATH;
private:
	CgiProcessor();
	CgiProcessor(const CgiProcessor& rhs);
	CgiProcessor&	operator=(const CgiProcessor& rhs);
	void	selectCgiCmd(string url);
	void	setURLEnv();
	void	setStartHeaderEnv();
	bool	isDirectory(const char *binPath);
	bool	checkErr(const char *binPath);
	bool	findCgiCmdPath();
	Request 			&request;
	ServerConfigData	*serverConfig;
	LocationConfigData	*locationConfig;
	string				pathEnv;
	string				scriptFile;
	string				cgiCommand;
	string				cgiContent;
	size_t				contentLength;
	bool				fin;
	map<string, string>			metaVariables;
	map<string, deque<string> > cgiResponseHeader;
};

#endif
