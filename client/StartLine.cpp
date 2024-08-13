/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Startline.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inghwang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 12:55:24 by inghwang          #+#    #+#             */
/*   Updated: 2024/07/17 12:55:25 by inghwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "StartLine.hpp"

extern int logs;

std::map<std::string, Method> originMethodInit()
{
    //config 파일에서 정해주기
    std::map<std::string, Method> m;

    m["GET"] = GET;
    // m["HEAD"] = HEAD;
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

StartLine::StartLine() : completion(false)
{}

StartLine::StartLine(const StartLine& src) : completion(src.getCompletion()), version(src.getVersion()), method(src.getMethod()), url(src.getUrl()), query(src.getQuery())
{}

StartLine&  StartLine::operator=(const StartLine& src)
{
    completion = src.getCompletion();
    method = src.getMethod();
    version = src.getVersion();
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

std::string  StartLine::getUrl() const
{
    return (url);
}

std::unordered_map<std::string, std::string>    StartLine::getQuery() const
{
    return (query);
}

int StartLine::urlQuery()
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
    return (0);
}

int     StartLine::plus(std::string temp)
{
    std::istringstream  strStream(temp);
    std::string         str;
    int                 answer;

    answer = 0;
    while (getline(strStream, str, ' '))
    {
        switch (answer)
        {
            case 0:
                method = originMethod[str];
                if (method == 0)
                    return (400);
                break ;
            case 1:
                if (str.empty())
                    return (400);
                url = str;
                if (urlQuery())
                    return (400);
                // url이 잘못된 형식이면 400 형식은 맞지만 존재하지 않는다면 404(Not Found)
                // 여기서 url검사와 allow검사 같이 진행하는 것이 좋을듯
                break ;
            case 2:
                version = originVersion[str];
                if (version == 0)
                    return (505);
                break ;
            default:
                return (400);
        }
        answer++;
    }
    if (answer != 3)
        return (400);
    completion = true;
    return (0);
}
