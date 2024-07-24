/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inghwang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 17:11:14 by inghwang          #+#    #+#             */
/*   Updated: 2024/07/08 17:11:16 by inghwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() : fd(-1)
{}

Client::Client(const Client& src) : fd(src.getFd()), msg(src.getMsg()), request(src.getRequest()), startline(src.getStartLine()), headerline(src.getHeaderline()), entityline(src.getEntity()), message(src.getMessage())
{}

Client& Client::operator=(const Client& src)
{
    fd = src.getFd();
    msg = src.getMsg();
    request = src.getRequest();
    startline = src.getStartLine();
    headerline = src.getHeaderline();
    entityline = src.getEntity();
    message = src.getMessage();
    return (*this);
}

Client::~Client()
{
    // close(fd);
    std::cout<<fd<<" client close"<<std::endl;
}

Client::Client(int fd) : fd(fd)
{}

int Client::getFd(void) const
{
    return (fd);
}

std::string Client::getMsg() const
{
    return (msg);
}

Request Client::getRequest() const
{
    return (request);
}

StartLine   Client::getStartLine() const
{
    return (startline);
}

HeaderLine  Client::getHeaderline() const
{
    return (headerline);
}

EntityLine    Client::getEntity() const
{
    return (entityline);
}

std::queue<std::string> Client::getMessage() const
{
    return (message);
}

bool    Client::getRequestFin() const
{
    return (request.fin);
}

void    Client::setFd(int fd)
{
    this->fd = fd;
}

bool    Client::getRequestFin()
{
    return (request.fin);
}

int Client::setStartLine(void)
{
    size_t      flag;

    if (message.size())
    {
        msg += message.back();
        message.pop();
    }
    // std::cout<<"msg: "<<msg<<"\n";
    flag = msg.find("\r\n");
    if (flag != std::string::npos)
    {
        if (startline.plus(msg.substr(0, flag)) < 0)
            return (-1);  //시작줄 에러
        msg = msg.substr(flag + 2);
        // std::cout<<"msg: "<<msg<<"\n";
        request.method = startline.getMethod();
        request.url = startline.getUrl();
        request.version = startline.getVersion();
    }
    return (0);
}

int Client::setHeader(void)
{
    size_t      flag;
    std::string str;

    if (message.size())
    {
        msg += message.back();
        message.pop();
    }
    while (1)
    {
        flag = msg.find("\r\n");
        if (flag != std::string::npos)
        {
            if (flag == 0)
            {
                if (headerline.headerError() < 0)
                    return (-2);  //vital header not or header double
                request.header = headerline.getHeader();
                msg = msg.substr(flag + 2);
                break ;
            }
            str = msg.substr(0, flag);
            if (headerline.plus(str) < 0)
                return (-1);  //헤더 에러
            msg = msg.substr(flag + 2);
        }
        else
            break ;
    }
    return (0);
}

int Client::setEntityLine(void)
{
    if (message.size())
    {
        msg += message.back();
        message.pop();
    }
    entityline.initContentLength(headerline.getContentLength());
    entityline.plus(msg, headerline.getEntitytype());
    request.entity = entityline.getEntity();
    return (0);
}

int Client::setTe(void)
{
    size_t      flag;
    std::string str;

    if (message.size())
    {
        msg += message.back();
        message.pop();
    }
    while (1)
    {
        if (msg.empty())
            break ;
        flag = msg.find("\r\n");
        if (flag != std::string::npos)
        {
            str = msg.substr(0, flag);
            if (headerline.checkTe(str) < 0)
                return (-1);
            msg = msg.substr(flag + 2);
        }
        else
            break ;
    }
    headerline.setTe(NOT);
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
    std::cout<<"=====header line=====\n";
    for (std::map<std::string, std::deque<std::string> >::iterator it = request.header.begin(); it != request.header.end(); it++)
    {
        std::cout<<it->first<<": ";
        for (itd = request.header[it->first].begin(); itd != request.header[it->first].end(); itd++)
            std::cout<<*itd<<"  ";
        std::cout<<"\n";
    }
    std::cout<<"=====entity line=====\n";
    for (std::vector<std::string>::iterator it = request.entity.begin(); it != request.entity.end(); it++)
    {
        std::cout<<*it;
    }
}

void    Client::setMessage(std::string str)
{
    message.push(str);
    if (!startline.getCompletion())
    {
        if (setStartLine() < 0)
            return ;  //시작줄 에러 처리하기
    }
    if (startline.getCompletion() && !headerline.getCompletion())
    {
        if (setHeader() < 0)
            return ;  //여기서 에러 처리하기
    }
    if (headerline.getCompletion() && !entityline.getCompletion())
    {
        if (setEntityLine() < 0)
            return ;   //여기서 에러 처리하기
    }
    if (entityline.getCompletion() && headerline.getTe() == YES)
    {
        if (setTe() < 0)
            return ; 
    }
    if (entityline.getCompletion() && headerline.getTe() == NOT && message.empty())
       request.fin = true;
    if (headerline.getEntitytype() == ENOT && message.empty())
        request.fin = true;
    //message 남아있을 경우에 에러 처리하기
}


