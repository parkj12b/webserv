/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 17:11:14 by inghwang          #+#    #+#             */
/*   Updated: 2024/08/13 18:27:32 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

extern int logs;

Client::Client() : fd(0), port(0), index(0), responseAmount(0), startLine(0), headerLine(0), contentLine(0)
{
    request.fin = false;
    request.status = 0;
}

Client::Client(int fd, int port) : fd(fd), port(port), index(0), responseAmount(0), startLine(port), headerLine(port), contentLine(port)
{
    request.fin = false;
    request.status = 0;
}

Client::Client(const Client& src) : fd(src.getFd()), port(src.getPort()), index(src.getIndex()), responseAmount(src.getResponseAmount()), msg(src.getMsg()), request(src.getRequest()), startLine(src.getStartLine()), headerLine(src.getHeaderline()), contentLine(src.getContentLine()), response(src.getResponse())
{}

Client& Client::operator=(const Client& src)
{
    fd = src.getFd();
    port = src.getPort();
    index = src.getIndex();
    responseAmount = src.getResponseAmount();
    msg = src.getMsg();
    request = src.getRequest();
    startLine = src.getStartLine();
    headerLine = src.getHeaderline();
    contentLine = src.getContentLine();
    response = src.getResponse();
    return (*this);
}

Client::~Client()
{
    std::cout<<fd<<" client close"<<std::endl;
}

int Client::getFd(void) const
{
    return (fd);
}

int Client::getPort(void) const
{
    return (port);
}

size_t  Client::getIndex() const
{
    return (index);
}

size_t  Client::getResponseAmount() const
{
    return (responseAmount);
}

std::string Client::getMsg() const
{
    return (msg.c_str() + index);
}

std::time_t Client::getKeepAlive() const
{
    return (keepAlive);
}

Request Client::getRequest() const
{
    return (request);
}

StartLine   Client::getStartLine() const
{
    return (startLine);
}

HeaderLine  Client::getHeaderline() const
{
    return (headerLine);
}

ContentLine    Client::getContentLine() const
{
    return (contentLine);
}

Response    Client::getResponse() const
{
    return (response);
}

bool    Client::getRequestFin() const
{
    return (request.fin);
}

int Client::getRequestStatus() const
{
    return (request.status);
}

void    Client::setFd(uintptr_t fd)
{
    this->fd = fd;
}

void    Client::setKeepAlive(std::time_t time)
{
    keepAlive = time;
}

void    Client::setRequestStatus(int temp)
{
    request.status = temp;
}

void    Client::setRequestFin(bool fin)
{
    request.fin = fin;
}

bool    Client::diffKeepAlive()
{
    if (difftime(std::time(0), keepAlive) > 10)
        return (false);
    return (true);
}

const char* Client::respondMsgIndex()
{
    return (msg.c_str() + index);
}

int Client::setStart(void)
{
    size_t      flag;

    if (startLine.getCompletion() || request.fin || request.status)
        return (0);
    std::cout<<"...startline parsing...\n";
    flag = msg.find("\r\n");
    if (flag != std::string::npos)
    {
        if ((request.status = startLine.check(msg.substr(0, flag))))  //ingu test
            return (1);
        msg = msg.substr(flag + 2);
        request.method = startLine.getMethod();
        request.url = startLine.getUrl();
        request.version = startLine.getVersion();
        request.query = startLine.getQuery();
    }
    else
    {
        if (msg.size() > 8192)  //config parser
        {
            request.status = 414;
            return (2);
        }
    }
    return (0);
}

int Client::setHeader(void)
{
    size_t      flag;
    std::string str;

    if (!startLine.getCompletion() || headerLine.getCompletion() || request.fin || request.status)
        return (0);
    std::cout<<"...headerline parsing...\n";
    while (1)
    {
        flag = msg.find("\r\n");
        if (flag != std::string::npos)
        {
            if (flag == 0)
            {
                if ((request.status = headerLine.headerError()) > 0)
                {
                    if (request.status == 100 && !msg.empty())
                        request.status = 0;
                }
                if (request.status > 0)
                {
                    std::cout<<"default error"<<std::endl;
                    return (2);
                }
                request.header = headerLine.getHeader();
                msg = msg.substr(flag + 2);
                contentLine.initContentLine(headerLine.getContentLength(), headerLine.getContentType());
                break ;
            }
            str = msg.substr(0, flag);
            if ((request.status = headerLine.makeHeader(str)) > 0)
            {
                std::cout<<str<<std::endl;
                return (1);  //400
            }
            msg = msg.substr(flag + 2);
            if (headerLine.getHeader().size() > 24576)
            {
                request.status = 400;
                std::cout<<"header size error"<<std::endl;
                return (2);  //400
            }
        }
        else
        {
            if (msg.size() > 8192)
            {
                request.status = 414;
                return (3);  //414
            }
            break ;
        }
    }
    if (headerLine.getCompletion())
    {
        //아직 다 들어오지 않은 데이터가 있을 수도 있잔녀 이건 우선 생각하지 않음
        //데이터가 후에 들어온다고 가정한다면 그때 가서 처리를 해주면 됨 하지만 들어오지 않고 eof가 들어오면 맞는 데이터임에도 error로 처리하기 때문에 여기서 이렇게 처리하는 것이 맡다. 
        if (request.method == GET)
            request.fin = true;
        else if (msg.empty() && headerLine.getContentType() == ENOT)
            request.fin = true;
        else if (!msg.empty() && headerLine.getContentType() == ENOT)
        {
            request.status = 400;
            return (1);
        }
    }
    return (0);
}

int Client::setContent(void)
{
    if (!headerLine.getCompletion() || contentLine.getCompletion() || request.fin || request.status)
        return (0);
    std::cout<<"...setBodyLine parsing...\n";
    if (contentLine.makeContentLine(msg) < 0)
    {
        request.status = 400;
        return (1);
    }
    request.content = contentLine.getContent();
    if (contentLine.getCompletion())
    {
        if (headerLine.getTe() == NOT)
        {
            if (!msg.empty())
            {
                request.status = 400;
                return (2);
            }
            else
                request.fin = true;
        }
    }
    return (0);
}

int Client::setTrailer(void)
{
    size_t      flag;
    std::string str;

    if (!contentLine.getCompletion() || headerLine.getTe() != YES || request.fin == true || request.status > 0)
        return (0);
    std::cout<<"...setTrailer parsing...\n";
    while (1)
    {
        if (msg.empty())
            break ;
        flag = msg.find("\r\n");
        if (flag != std::string::npos)
        {
            str = msg.substr(0, flag);
            msg = msg.substr(flag + 2);
            if ((request.status = headerLine.parseTrailer(str)) > 0)
                return (1);
            else
            {
                request.header = headerLine.getHeader();
                if (request.header["trailer"].empty())
                {
                    headerLine.setTrailer(NOT);
                    request.fin = true;
                    break ;
                }
            }
        }
        else
        {
            if (msg.size() > 8192)
            {
                request.status = 414;
                return (2);  //414
            }
            break ;
        }
    }
    return (0);
}


//temp(must delete)
void    Client::showMessage(void)
{
    std::deque<std::string>::iterator  itd;

    time_t now = time(0);
    // time_t 형식을 문자열로 변환합니다.
    char* dt = ctime(&now);
    std::cout<<"time : "<<dt;
    //request 출력
    std::cout<<"=====strat line=====\n";
    std::cout<<"fd : "<<fd<<std::endl;
    std::cout<<request.method<<" "<<request.version<<" "<<request.url<<std::endl;
    for (std::unordered_map<std::string, std::string>::iterator it = request.query.begin(); it != request.query.end(); it++)
        std::cout<<it->first<<"="<<it->second<<std::endl;
    std::cout<<"=====header line=====\n";
    for (std::unordered_map<std::string, std::deque<std::string> >::iterator it = request.header.begin(); it != request.header.end(); it++)
    {
        std::cout<<it->first<<": ";
        for (itd = request.header[it->first].begin(); itd != request.header[it->first].end(); itd++)
            std::cout<<*itd<<"  ";
        std::cout<<"\n";
    }
    std::cout<<"=====entity line=====\n";
    for (std::vector<std::string>::iterator it = request.content.begin(); it != request.content.end(); it++)
    {
        std::cout<<*it;
    }
    std::cout<<"\n\n";
}

void    Client::setMessage(std::string msgRequest)
{
    msg += msgRequest;
    write(logs, &msgRequest[0], msgRequest.size());
    if (setStart())  //max size literal
    {
        std::cout<<fd<<" "<<request.status<<" ";
        std::cout<<"Startline Error\n";
        return ;
    }
    if (setHeader())  //max size literal, 헤더 파싱
    {
        std::cout<<"Header Error\n";
        return ;
    }
    if (setContent()) // 바디 파싱
    {
        std::cout<<"Body Error\n";
        return ;
    }
    if (setTrailer()) // 바디 마지막, 트레일러 파싱
    {
        std::cout<<"Trailer Error\n";
        return ; 
    }
}

void    Client::setResponseMessage()
{
    index = 0;
    response.initRequest(request);
    response.responseMake();
    msg = response.getEntity();
    responseAmount = msg.size();
}

size_t  Client::responseIndex()
{
    if (responseAmount <= index)
        responseAmount = index;
    return (responseAmount - index);
}

void    Client::plusIndex(size_t plus)
{
    index += plus;
}
