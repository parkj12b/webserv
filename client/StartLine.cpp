/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StartLine.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 12:55:24 by inghwang          #+#    #+#             */
/*   Updated: 2024/09/07 17:33:36 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "StartLine.hpp"
#include "UtilTemplate.hpp" 

std::map<std::string, Method> originMethodInit()
{
    //config 파일에서 정해주기
    std::map<std::string, Method> m;

    m["GET"] = GET;
    m["POST"] = POST;
    m["DELETE"] = DELETE;
    return (m);
}

std::map<std::string, Version> originVersionInit()
{
    std::map<std::string, Version> m;

    m["HTTP/1.1"] = HTTP11;
    return (m);
}

// 여기에 정의를 추가하세요
std::map<std::string, Method> StartLine::originMethod = originMethodInit();
std::map<std::string, Version> StartLine::originVersion = originVersionInit();
std::string StartLine::methodString[4] = {"", "GET", "POST", "DELETE"};

StartLine::StartLine() : completion(false), port(0)
{}

StartLine::StartLine(const StartLine& src) : completion(src.getCompletion()), version(src.getVersion()), method(src.getMethod()), port(src.getPort()), url(src.getUrl()), query(src.getQuery())
{}

StartLine&  StartLine::operator=(const StartLine& src)
{
    completion = src.getCompletion();
    version = src.getVersion();
    method = src.getMethod();
    port = src.getPort();
    url = src.getUrl();
    query = src.getQuery();
    return (*this);
}

StartLine::~StartLine()
{
    (void) port;
}

StartLine::StartLine(int port) : completion(false), port(port)
{}

bool        StartLine::getCompletion() const
{
    return (completion);
}

Method  StartLine::getMethod() const
{
    return (method);
}

Version StartLine::getVersion() const
{
    return (version);
}

int StartLine::getPort() const
{
    return (port);
}

std::string  StartLine::getUrl() const
{
    return (url);
}

std::string StartLine::getLocation() const
{
    return (location);
}

std::map<std::string, std::string>    StartLine::getQuery() const
{
    return (query);
}

void    StartLine::urlQuery()
{
    size_t      pos;
    std::string queryTemp;
    std::string str;

    pos = url.find('#');
    if (pos != std::string::npos)
        url.substr(0, pos);
    pos = url.find('?');
    if (pos != std::string::npos)
    {
        queryTemp = url.substr(pos + 1);
        url = url.substr(0, pos);
        //config parser
        //여기서 url검사: method를 사용할 수 있는지 등등, get은 존재하는지
        std::istringstream  strStream(queryTemp);
        while (getline(strStream, str, '&'))
        {
            pos = str.find('=');
            if (pos != std::string::npos)
                query[str.substr(0, pos)] = str.substr(pos + 1);
        }
    }
}

int     StartLine::check(std::string firstLine)
{
    std::istringstream  strStream(firstLine);
    std::string         str;
    int                 answer;

    answer = 0;
    LOG(std::cout<<port<<std::endl);
    while (getline(strStream, str, ' '))
    {
        if (str.empty())
            return (400);
        switch (answer)
        {
            case 0:
                method = originMethod[str];
                if (method == 0)
                    return (405);
                break ;
            case 1:  //allow checking and root checking (config parser)
                url = str;  //.찾으면 지우기
                urlQuery();
                break ;
            case 2:
                version = originVersion[str];
                if (version == 0)
                    return (505);
                break ;
        }
        answer++;
    }
    if (answer != 3)
        return (400);
    completion = true;
    return (0);
}
