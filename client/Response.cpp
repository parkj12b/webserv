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

std::map<int, std::string>  statusContentInit()
{
    std::map<int, std::string>  m;

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

std::map<int, std::string>  Response::statusContent = statusContentInit();

void    Response::makeFilePath(std::string& str)
{
    LocationConfigData *location = getLocationConfigData();

    cout << "host: " << request.header["host"].front() << endl;
    cout << "str before: " << str << endl;
    str = location->getRoot() + "/" + str;
    if (isDirectory(str.c_str()))
    {
        // 없으면 index.html 이라 없을 일은 없음.
        cout << "index: " << location->getIndex() << endl;
        if (str[str.size() - 1] == '/')
            str += location->getIndex();
        else
        {
            cout << "str: " << str << endl;
            request.status = 404;
            return ;
        }
    }
    cout << "str: " << str << endl;
    if (access(str.c_str(), F_OK | R_OK) == -1)
    {
        request.status = 404;
        return ;
    }

    // pos = str.find("http");
    // if (pos == 0)
    // {
    //     str = str.substr(8);
    //     pos = str.find('/');
    //     str = str.substr(pos);
    // }
    // if (str[0] == '/')
    //     str = "." + str;
    // else
    //     str = "./" + str;
}

Response::Response()
{}

Response::Response(const Response& src)
{
    start = src.getStart();
    header = src.getHeader();
    content = src.getContent();
    entity = src.getEntity();
    request = src.getRequest();
    port = src.getPort();
}

Response&    Response::operator=(const Response& src)
{
    if (this == &src)
        return (*this);
    start = src.getStart();
    header = src.getHeader();
    content = src.getContent();
    entity = src.getEntity();
    request = src.getRequest();
    port = src.getPort();
    return (*this);
}

Response::~Response()
{
}

Response::Response(int port) : port(port)
{}

int Response::getPort() const
{
    return (port);
}

std::string Response::getStart() const
{
    return (start);
}

std::string Response::getHeader() const
{
    return (header);
}

std::string Response::getContent() const
{
    return (content);
}

std::string Response::getEntity() const
{
    return (entity);
}

Request Response::getRequest() const
{
    return (request);
}

void    Response::setRequest(Request &temp)
{
    request = temp;
}

void    Response::initRequest(Request msg)
{
    request = msg;
}

void    Response::init()
{
    cout << "port: " << port << endl;

    start.clear();
    header.clear();
    content.clear();
    entity.clear();
    string host = request.header["host"].front();
    cout << "host: " << host << endl;
    try
    {
        serverConfig = Server::serverConfig->getServerData(host, port);
    }
    catch(const std::exception& e)
    {
        serverConfig = Server::serverConfig->getDefaultServer(port);
        if (serverConfig == NULL)
            request.status = 400;
    }
    makeHeader("Server", "inghwang/0.0");
    if (request.header["cookie"].empty())
        makeHeader("Set-Cookie", "session_id=abc123");
}

int Response::getDefaultErrorPage(int statusCode)
{
    if (statusCode >= 400 && statusCode < 500)
        return (open(DEFAULT_400_ERROR_PAGE, O_RDONLY));
    else if (statusCode >= 500)
        return (open(DEFAULT_500_ERROR_PAGE, O_RDONLY));
    return (open(DEFAULT_400_ERROR_PAGE, O_RDONLY));
}

void    Response::makeDate()
{
    time_t      now;
    char*       dt;

    now = time(0);
    dt = ctime(&now);
    std::string         date;
    std::string         str(dt);
    std::istringstream  strStream(str);
    std::string         temp;
    std::string         day[5];
    size_t              pos;
    int                 order;

    order = 0;
    while (std::getline(strStream, temp, ' '))
    {
        pos = temp.find_last_not_of('\n');
        temp.erase(pos + 1);
        day[order++] = temp;
    }
    date = day[0] + ", " + day[2] + " " + day[1] + " " + day[4] + " " + day[3] + " GMT";
    makeHeader("Date", date);
}

void    Response::makeError()
{
    //url 이 필요함 -> url 파싱해야됨, prefix match 
    LocationConfigData   *location = getLocationConfigData();
    map<int, string>   &errorPage = location->getErrorPage();

    (void) errorPage;
    int fd;

    
    if (request.status >= 300 && request.status < 400)
        return ;
    start = "HTTP/1.1 " + std::to_string(request.status) + statusContent[request.status] + "\r\n";
    if (request.status == 100)
        return ;
    fd = getDefaultErrorPage(request.status);
    if (fd < 0)
        return ;
    makeHeader("content-type", "text/html");
    makeContent(fd);
}

void    Response::makeHeader(std::string key, std::string value)
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
    std::cout<<content.size()<<std::endl;
    makeHeader("content-length", std::to_string(count));
    close(fd);
}

void    Response::makeEntity()
{
    entity = start;
    if (!header.empty())
        entity += header + "\r\n";
    std::cout<<entity.size()<<std::endl;
    if (!content.empty())
        entity.append(content);
    std::cout<<entity.size()<<std::endl;
}

void    Response::makeGet()
{
    int fd;

    std::cout<<"Method: GET"<<std::endl;
    std::cout<<request.url.c_str()<<std::endl;
    //cgi checking...
    fd = open(request.url.c_str(), O_RDONLY);
    if (fd < 0)
    {
        request.status = 404;
        start = "HTTP1.1 " + std::to_string(request.status) + statusContent[request.status] + "\r\n";
        fd = open(DEFAULT_400_ERROR_PAGE, O_RDONLY);
        if (fd < 0)
            return ;
        //거기에 맞는 content만들기
        makeHeader("Content-Type", "text/html");
        makeContent(fd);
        return ;
    }
    makeContent(fd);
    request.status = 200;
    start = "HTTP1.1 " + std::to_string(request.status) + statusContent[request.status] + "\r\n";
}


void    Response::makePost()
{
    std::string buffer;
    int         fd;

    std::cout<<"Method: POST"<<std::endl;
    //cgi checking...
    fd = open(request.url.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0777);
    if (fd < 0)
    {
        request.status = 404;
        makeError();
        return ;
    }
    for (std::vector<std::string>::iterator it = request.content.begin(); it != request.content.end(); it++)
    {
        buffer = *it;
        if (write(fd, &buffer[0], buffer.size()) < static_cast<int>(buffer.size()))
        {
            request.status = 500;
            makeError();
            return ;
        }
    }
    close(fd);
    request.status = 204;
    start = "HTTP1.1 " + std::to_string(request.status) + statusContent[request.status] + "\r\n";
}

void    Response::makeDelete()
{
    std::cout<<"Method: DELETE"<<std::endl;
    if (std::remove(request.url.c_str()) == 0)
    {
        request.status = 204;
        start = "HTTP1.1 " + std::to_string(request.status) + statusContent[request.status] + "\r\n";
    }
    else
    {
        request.status = 404;
        makeError();
    }
    //접근 권한이 실패될 경우에는 403
}

void    Response::responseMake()
{
    
    init();
    makeDate();
    checkAllowedMethod();
    if (request.status > 0)
    {
        makeError();
        makeEntity();
        return ;
    }
    checkRedirect();
    if (request.status > 0)
        return (makeEntity());
    makeFilePath(request.url);
    if (request.status > 0)
    {
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

void    Response::checkAllowedMethod()
{
    LocationConfigData  *location = getLocationConfigData();
    vector<string>    &allowedMethods = location->getAllowedMethods();

    if (find(allowedMethods.begin(), allowedMethods.end(),
        StartLine::methodString[request.method]) == allowedMethods.end())
        request.status = 405;
}

LocationConfigData *Response::getLocationConfigData()
{
    return (locationConfig);
}

void    Response::setLocationConfigData(LocationConfigData *locationConfigData)
{
    locationConfig = locationConfigData;
}

void    Response::checkRedirect()
{
    LocationConfigData  *location = getLocationConfigData();
    pair<int, string>   &redirect = location->getReturn();

    if (redirect.first != 0)
    {
        request.status = redirect.first;
        request.url = redirect.second;
    }
    cout << request.url << endl;
    cout << request.status << endl;
    makeHeader("Location", redirect.second);
    start = "HTTP/1.1 " + std::to_string(request.status) + statusContent[request.status] + "\r\n";
}
