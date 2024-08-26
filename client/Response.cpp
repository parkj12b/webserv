/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inghwang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 16:25:04 by inghwang          #+#    #+#             */
/*   Updated: 2024/08/09 16:25:05 by inghwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include "Response.hpp"
#include "Server.hpp"
#include "LocationConfigData.hpp"
#include "ServerConfigData.hpp"
#include "Trie.hpp"
#include "UtilTemplate.hpp"
#include "StartLine.hpp"

using namespace std;

map<int, string>  statusContentInit()
{
    map<int, string>  m;

    m[100] = " Continue";
    m[101] = " Switching Protocols";
    m[102] = " Processing";
    m[103] = " Early Hints";
    m[200] = " OK";
    m[201] = " Created";
    m[202] = " Accepted";
    m[203] = " Non-Authoritative Information";
    m[204] = " No Content";
    m[205] = " Reset Content";
    m[206] = " Partial Content";
    m[207] = " Multi-Status";
    m[208] = " Already Reported";
    m[214] = " Transformation Applied";
    m[226] = " IM Used";
    m[300] = " Multiple Choices";
    m[301] = " Moved Permanently";
    m[302] = " Found";
    m[303] = " See Other";
    m[304] = " Not Modified";
    m[305] = " Use Proxy";
    m[307] = " Temporary Redirect";
    m[308] = " Permanent Redirect";
    m[400] = " Bad Request";
    m[401] = " Unauthorized";
    m[402] = " Payment Required";
    m[403] = " Forbidden";
    m[404] = " Not Found";
    m[405] = " Method Not Allowed";
    m[406] = " Not Acceptable";
    m[407] = " Proxy Authentication Required";
    m[408] = " Request Timeout";
    m[409] = " Conflict";
    m[410] = " Gone";
    m[411] = " Length Required";
    m[412] = " Precondition Failed";
    m[413] = " Payload Too Large";
    m[414] = " Request-URI Too Long";
    m[415] = " Unsupporred Media Type";
    m[416] = " Request Range Not Staisfiable";
    m[417] = " Expectation Failed";
    m[418] = " I’m a teapot";
    m[420] = " Enhance Your Calm";
    m[421] = " Misdirected Request";
    m[422] = " Unprocessable Entity";
    m[423] = " Locked";
    m[424] = " Failed Dependency";
    m[425] = " Too Early";
    m[426] = " Upgrade Required";
    m[428] = " Precondition Required";
    m[429] = " Too many Requests";
    m[431] = " Request Header Fields Too Large";
    m[444] = " No Response";
    m[450] = " Blocked by Windows Parental Controls";
    m[451] = " Unavailable For Legal Reasons";
    m[497] = " HTTP Request Sent to HTTPS Port";
    m[498] = " Token expired/invalid";
    m[499] = " Client Closed Request";
    m[500] = " Internal Server Error";
    m[501] = " Not Implemented";
    m[502] = " Bad Gateway";
    m[503] = " Service Unavailable";
    m[504] = " Gateway Timeout";
    m[506] = " Variant Also Negotiates";
    m[507] = " Insufficient Storage";
    m[508] = " Loop Detected";
    m[509] = " Bandwidth Limit Exceeded";
    m[510] = " Not Extended";
    m[511] = " Network Authentication Required";
    m[521] = " Web Server Is Down";
    m[522] = " Connection Timed Out";
    m[523] = " Origin Is Unreachable";
    m[525] = " SSL Handshake Failed";
    m[530] = " Site Frozen";
    m[599] = " Network Connect Timeout Error";
    return (m);
}

map<string, string>  sessionInit()
{
    map<string, string>  m;

    return (m);
}

map<int, string>  Response::statusContent = statusContentInit();
map<string, string>  Response::session = sessionInit();

bool	Response::isCgiScriptInURL(string& str)
{
	const string	availCgiExtensions[2] = {".py", ".php"};
	size_t			cgiFilePos;
	for (int i=0; i<2; i++)
	{
		cgiFilePos = str.find(availCgiExtensions[i]);
		if (cgiFilePos != string::npos)
			break ;
	}
	if (cgiFilePos == string::npos)
	{
		if (request.method == POST)
			request.status = 400;
		return (false);
	}
	return (true);
}

void    Response::makeFilePath(string& str)
{
    LocationConfigData *location = getLocationConfigData();

    // LOG(cout << "host: " << request.header["host"].front() << endl);
    LOG(cout << "str before: " << str << endl);
    str = location->getRoot() + "/" + str;
    if (isWithinBasePath(location->getRoot(), str) == false)
    {
        request.status = 403;
        LOG(cout << "403 1\n");
        return ;
    }
    if (str[str.size() - 1] == '/' && isDirectory(str.c_str()))
    {
        // 없으면 index.html 이라 없을 일은 없음.
        LOG(cout << "index: " << location->getIndex() << endl);
        string temp = str;
        str += "/" + location->getIndex();
        if (isFile(str.c_str()) == false)
        {
            if (location->getAutoindex())
                str = temp + "/";
            else
            {
                cout << "403 2\n";
                request.status = 403;
            }
            return ;
        }
    }
    else if (isFile(str.c_str()) == false)
    {
        LOG(cout << "not file: " << str << endl);
        request.status = 404;
        return ;
    }
	if (isCgiScriptInURL(str))
		cgiFlag = true;
    LOG(cout << "str: " << str << endl);
}

Response::Response() : cgiFlag(false)
{
}

Response::Response(const Response& src)
{
    *this = src;
}

Response&    Response::operator=(const Response& src)
{
    if (this == &src)
        return (*this);
    cgiFlag = src.getCgiFlag();
    port = src.getPort();
    startHeaderLength = src.getStartHeaderLength();
    contentLength = src.getContentLength();
    start = src.getStart();
    header = src.getHeader();
    content = src.getContent();
    entity = src.getEntity();
    request = src.getRequest();
    keyHeader = src.getKeyHeader();
    // serverConfig = src.getServerConfig();
    locationConfig = src.getLocationConfigData();
    return (*this);
}

Response::~Response()
{
}

Response::Response(int port, string pathEnv_) : cgiFlag(false), port(port), contentLength(0), pathEnv(pathEnv_)
{}

int Response::getPort() const
{
    return (port);
}

size_t  Response::getStartHeaderLength() const
{
    return (startHeaderLength);
}

size_t  Response::getContentLength() const
{
    return (contentLength);
}

string Response::getStart() const
{
    return (start);
}

string Response::getHeader() const
{
    return (header);
}

string Response::getContent() const
{
    return (content);
}

string Response::getEntity() const
{
    return (entity);
}

Request Response::getRequest() const
{
    return (request);
}

int Response::getRequestStatus() const
{
    return (request.status);
}

bool	Response::getCgiFlag() const
{
	return (cgiFlag);
}

std::vector<std::string>    Response::getKeyHeader() const
{
    return (keyHeader);
}

LocationConfigData *Response::getLocationConfigData() const
{
    return (locationConfig);
}

void    Response::setCgiFlag(bool flag)
{
    cgiFlag = flag;
}

void        Response::setPort(int port)
{
    this->port = port;
}

void	Response::setPathEnv(string pathEnv_)
{
	pathEnv = pathEnv_;
}

void    Response::setRequest(Request &temp)
{
    request = temp;
}

void    Response::setRequestStatus(int status)
{
    request.status = status;
}

void    Response::setLocationConfigData(LocationConfigData *locationConfigData)
{
    locationConfig = locationConfigData;
}

size_t  Response::setContent(string content_)
{
    size_t      crlfPos;
    size_t      contentPos;
    std::string contentType;

    crlfPos = content_.find("\r\n");
    if (crlfPos != std::string::npos)
    {
        contentType = content_.substr(0, crlfPos);
        content = content_.substr(crlfPos + 2);
        contentPos = contentType.find(":");
        if (contentPos != std::string::npos)
            makeHeader("content-type", contentType.substr(contentPos + 1));
    }
    else
    {
        content = content_;
        return (0);
    }
    // LOG(std::cout<< "request.status: "<<request.status<<std::endl);
    // LOG(std::cout<<entity<<std::endl<<std::endl);
    // LOG(std::cout<<"================"<<std::endl);
    return (crlfPos + 2);
}

void	Response::setContentLength(size_t contentLength_)
{
    contentLength = contentLength_;
    makeHeader("content-length", toString(contentLength));
    makeEntity();
    // LOG(std::cout<<"header: \n\n"<<header);
    // LOG(std::cout<<"================"<<std::endl);
}

void    Response::initRequest(Request msg)
{
    request = msg;
    // LOG(cout<<request.fin<<endl);
    // LOG(cout<<request.status<<endl);
    // LOG(cout<<request.port<<endl);
    // LOG(cout<<request.clientFd<<endl);
    // LOG(cout<<request.method<<endl);
    // LOG(cout<<request.clientIp<<endl);
    // LOG(cout<<request.url<<endl);
    // LOG(cout<<request.location<<endl);
    // LOG(cout<<request.version<<endl);
}

int Response::init()
{
    LOG(cout << "port: " << port << endl);
    cgiFlag = false;
    if (request.status != 0)
        return (0);
    start.clear();
    header.clear();
    content.clear();
    entity.clear();
    string host = request.header["host"].front();
    LOG(cout << "host : " << host << endl);
	cgiFlag = false;
    // LOG(cout << "host: " << host << endl);
    try
    {
        serverConfig = Server::serverConfig->getServerData(host, port);
    }
    catch(const exception& e)
    {
        serverConfig = Server::serverConfig->getDefaultServer(port);
        if (serverConfig == NULL)
            request.status = 400;
        makeError();
        makeEntity();
        return (1);
    }
    return (0);
}

int Response::getDefaultErrorPage(int statusCode)
{
    string errorStr = getLocationConfigData()->getErrorPage()[statusCode];

    int fd = -1;

    if (errorStr.size() > 0)
        fd = open(errorStr.c_str(), O_RDONLY);
    if (fd != -1)
        return (fd);
    if (statusCode >= 400 && statusCode < 500)
        return (open(DEFAULT_400_ERROR_PAGE, O_RDONLY));
    if (statusCode >= 500)
        return (open(DEFAULT_500_ERROR_PAGE, O_RDONLY));
    return (open(DEFAULT_400_ERROR_PAGE, O_RDONLY));
}

void    Response::makeCookie(string& date)
{
    const string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    const size_t charactersSize = characters.size();
    string result;
    size_t      index;
    string value;
    string cookieValue;

    if (request.header["cookie"].empty())
    {
        for (size_t i = 0; i < 12; ++i)
        {
            index = rand() % charactersSize;
            result += characters[index];
        }
        session[result] = date;
        value = "session_id=" + result + "; Max-Age=3600";  //1시간
        // value = "session_id=" + result + "; Max-Age=60";  //1분
        makeHeader("Set-Cookie", value);
    }
    else
    {
        cookieValue = request.header["cookie"].front();
        index = cookieValue.find(';');
        if (index != string::npos)
            cookieValue = cookieValue.substr(0, index);
        index = cookieValue.find('=');
        if (index != string::npos)
            cookieValue = cookieValue.substr(index + 1);
        HeaderLine::eraseSpace(cookieValue, 0);
        if (session.find(cookieValue) == session.end())
        {
            session[cookieValue] = date;
        }
        // LOG(std::cout<<"cookieValue: "<<cookieValue<<std::endl<<std::endl);
        makeHeader("session", session[cookieValue]);
    }
}

void    Response::makeDefaultHeader()
{
    time_t              now;
    char*               dt;

    now = time(0);
    dt = ctime(&now);
    std::string         date;
    std::string         str(dt);
    std::ostringstream  oss(str);
    std::istringstream  strStream(str);
    std::string         temp;
    std::string         day[5];
    size_t              pos;
    int                 order;

    order = 0;
    while (getline(strStream, temp, ' '))
    {
        pos = temp.find_last_not_of('\n');
        temp.erase(pos + 1);
        day[order++] = temp;
    }
    date = day[0] + ", " + day[2] + " " + day[1] + " " + day[4] + " " + day[3] + " GMT";
    makeHeader("Date", date);
    makeHeader("Server", "inghwang/0.0");
    makeHeader("connection", "keep-alive");
    makeCookie(date);
}

void    Response::makeError()
{
    LOG(cout << "makeError\n"<<request.status<<std::endl);
    if (request.status >= 300 && request.status < 400)
        return ;
    if (request.status == 100)
        return ;
	LOG(cout << request.clientFd << std::endl);
    LocationConfigData	*location = getLocationConfigData();
    map<int, string>	&errorPage = location->getErrorPage();
    string errorPath = errorPage[request.status];

    LOG(cout << "errorPath: " << errorPath << endl);
    int fd = -1;
    if (errorPath != "")
        fd = open(errorPath.c_str(), O_RDONLY);
    if (errorPath != "" || fd >= 0)
        makeContent(fd);
    else
    {
        CgiProcessor cgiProcessor(request, serverConfig, locationConfig, pathEnv);
		cgiProcessor.selectCgiCmd(CGI_ERROR_PAGE);
		cgiProcessor.insertEnv("ERROR_CODE", toString(request.status));
        cgiProcessor.executeCGIScript(cgiProcessor.getScriptFile());
        setCgiFlag(true);
    }
}

int Response::checkRedirect()
{
    LocationConfigData  *location = getLocationConfigData();
    pair<int, string>   &redirect = location->getReturn();

    if (redirect.first != 0)
    {
        request.status = redirect.first;
        request.url = redirect.second;
        LOG(cout << request.url << endl);
        LOG(cout << request.status << endl);
        makeHeader("Location", redirect.second);
        // start = "HTTP/1.1 " + to_string(request.status) + statusContent[request.status] + "\r\n";
    }
    if (request.status > 0)
    {
        makeEntity();
        return (1);
    }
    return (0);
}

int Response::checkAllowedMethod() 
{
    LocationConfigData  *location = getLocationConfigData();
    vector<string>    &allowedMethods = location->getAllowedMethods();
    LOG(cout << allowedMethods.size() << endl);
    // for (auto iter = allowedMethods.begin(); iter != allowedMethods.end(); iter++)
    //     LOG(cout << "allowed method : " << endl);

    if (find(allowedMethods.begin(), allowedMethods.end(),
        StartLine::methodString[request.method]) == allowedMethods.end())
    {
        request.status = 405;
        makeError();
        makeEntity();
        return (1);
    }
    return (0);
}

void    Response::makeHeader(string key, string value)
{
    header += key + ": " + value + "\r\n";
}

void    Response::makeContent(int fd)
{
    int     count;
    int     readSize;
    char    buffer[4096];

    if (request.url == "./favicon.ico")
        makeHeader("Content-Type", "image/x-icon");
    else
        makeHeader("Content-Type", "text/html");
    count = 0;
    while (1)
    {
        readSize = read(fd, buffer, 4095);
        if (readSize <= 0)
            break ;
        content.append(buffer, readSize);
        count += readSize;
    }
    LOG(cout<<"Content Size: "<<content.size()<<endl);
    contentLength = count;
    makeHeader("content-length", to_string(count));
    close(fd);
}

void    Response::makeEntity()
{
    entity.clear();
    // LOG(std::cout<<request.status<<std::endl);
    if (request.status == 0)
        request.status = 200;
    entity = "HTTP/1.1 " + to_string(request.status) + statusContent[request.status] + "\r\n";
    if (!header.empty())
        entity += header + "\r\n";
    startHeaderLength = entity.size();
    if (!content.empty())
        entity.append(content);
    // LOG(std::cout<<"request.status: "<<request.status<<std::endl);
    // LOG(cout<<entity.size()<<endl);
}

void    Response::makeGet()
{
    int fd;

    LOG(std::cout<<"Method: GET"<<std::endl);
    LOG(std::cout<<request.url.c_str()<<std::endl);
    CgiProcessor cgiProcessor(request, serverConfig, locationConfig, pathEnv);
    
    // LOG(cout << "is directory: " << isDirectory(request.url.c_str()) << endl);
    // LOG(cout << "location: " << getLocationConfigData()->getPath() << endl);
    // LOG(cout << "autoindex: " << getLocationConfigData()->getAutoindex() << endl);

    //directory 검사는 makeFilePath 에서 함
    // LOG(cout << "request url: " << request.url << endl);
    if (isDirectory(request.url.c_str()))
    {
        cgiFlag = true;
        cgiProcessor.selectCgiCmd(AUTOINDEX_PATH);
        LOG(cout << "directory listing" << endl);
    	cgiProcessor.executeCGIScript(cgiProcessor.getScriptFile());
    }
	else if (cgiFlag)
	{
		cgiProcessor.selectCgiCmd(request.url);
		cgiProcessor.executeCGIScript(cgiProcessor.getScriptFile());
		if (request.status >= 400)
		{
			while (!cgiProcessor.getFin())
				cgiProcessor.executeCGIScript(CGI_ERROR_PAGE);
		}
		// 나중에 content-length가 0일 때, 서버 에러 추가
		start = "HTTP1.1 " + to_string(request.status) + statusContent[request.status] + "\r\n";
		// makeHeader("content-type", "text/html");
		// makeHeader("content-length", toString(contentLength)); //여기서 추가하고 나중에 또 추가함
		makeHeader("status", toString(request.status));
		content += cgiProcessor.getCgiContent();
		LOG(cout << cgiProcessor.getCgiContent() << '\n');
        // LOG(std::cout<<header);
	}
	else
	{
		fd = open(request.url.c_str(), O_RDONLY);
		if (fd < 0)
		{
			request.status = 404;
			start = "HTTP1.1 " + to_string(request.status) + statusContent[request.status] + "\r\n";
			while (!cgiProcessor.getFin())
				cgiProcessor.executeCGIScript(CGI_ERROR_PAGE);
            makeHeader("Content-Type", "text/html");
			content += cgiProcessor.getCgiContent();
            LOG(cout << cgiProcessor.getCgiContent() << '\n');
            // fd = open(DEFAULT_400_ERROR_PAGE, O_RDONLY);
			// if (fd < 0)
			// 	return ;
			// //거기에 맞는 content만들기
			// makeHeader("Content-Type", "text/html");
			// makeContent(fd);
			return ;
		}
		makeContent(fd);
	}
	request.status = 200;
    // start = "HTTP1.1 " + to_string(request.status) + statusContent[request.status] + "\r\n";
}

void    Response::makePost()
{
    LOG(cout<<"Method: POST"<<endl);
	CgiProcessor cgiProcessor(request, serverConfig, locationConfig, pathEnv);
	if (cgiFlag)
	{
		cgiProcessor.selectCgiCmd(request.url);
		cgiProcessor.checkPostContentType();
	}
	if (request.status >= 400)
	{
		while (!cgiProcessor.getFin())
			cgiProcessor.executeCGIScript(CGI_ERROR_PAGE);
	}
}

void    Response::makeDelete()
{
    LOG(cout<<"Method: DELETE"<<endl);
    if (remove(request.url.c_str()) == 0)
    {
        request.status = 204;
    }
    else
    {
        request.status = 404;
        makeError();
    }
}

void    Response::responseMake()
{
    if (request.status > 0)
    {
        makeError();
        makeEntity();
        return ;
    }
    if (init())
        return ;
    // LOG(cout << "request.status: " << request.status << endl);
    makeDefaultHeader();
    if (checkAllowedMethod())
        return ;
    // LOG(cout << "path: " << locationConfig->getPath() << endl);
    if (checkRedirect())
        return ;
    makeFilePath(request.url);
    if (request.status > 0)
    {
        LOG(std::cout<<"ERROR\n"<<std::endl);
        makeError();
        makeEntity();
        return ;
    }
    switch (request.method)
    {
        case GET:
            makeGet();
            break ;
        case POST:
            makePost();
            break ;
        case DELETE:
            makeDelete();
            break ;
        default:
            break ;
    }
    makeEntity();
    return ;
}

