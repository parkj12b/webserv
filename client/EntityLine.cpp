/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EntityLine.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inghwang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 16:24:10 by inghwang          #+#    #+#             */
/*   Updated: 2024/07/20 16:24:12 by inghwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EntityLine.hpp"

extern int logs;

EntityLine::EntityLine()
{
    completion = false;
    sizeEqual = false;
}

EntityLine::EntityLine(const EntityLine& src)
{
    completion = src.getCompletion();
    sizeEqual = src.getSizeEqual();
    contentLength = src.getContentLength();
    chunked = src.getChunked();
    entity = src.getEntity();
}

EntityLine::~EntityLine()
{}

EntityLine& EntityLine::operator=(const EntityLine& src)
{
    completion = src.getCompletion();
    entity = src.getEntity();
    return (*this);
}

bool    EntityLine::getCompletion() const
{
    return (completion);
}

bool    EntityLine::getSizeEqual() const
{
    return (sizeEqual);
}

int EntityLine::getContentLength() const
{
    return (contentLength);
}

std::string EntityLine::getChunked() const
{
    return (chunked);
}

std::vector<std::string>    EntityLine::getEntity() const
{
    return (entity);
}

void    EntityLine::initContentLength(int init)
{
    contentLength = init;
}

void    EntityLine::minusContentLength(int minus)
{
    contentLength -= minus;
}

int EntityLine::chunkedEntity()
{
    std::istringstream  chunkedStream(chunked);
    std::string         temp;
    int                 ans;
    int                 size;

    ans = 0;
    //chunked인데  \r\n이 아니라 \n
    while (getline(chunkedStream, temp))
    {
        if (temp[temp.size() - 1] != '\r')
            return (-1);
        if (temp[temp.size() - 1] == '\r') //temp[temp.size() - 1] == '\r'
            temp.erase(temp.size() - 1);
        if (ans % 2 == 0)
            size = std::stoi(temp, nullptr, 16);
        else
        {
            if (size != static_cast<int>(temp.size()))
                return (-2);
            entity.push_back(temp);
        }
        ans++;
    }
    completion = true;
    return (0);
}

int EntityLine::plus(std::string &str, ENTITYTYPE entitytype)
{
    size_t  flag;

    if (entitytype == CONTENT)
    {
        std::cout<<contentLength<<' '<<str.size()<<std::endl;
        if (contentLength >= static_cast<int>(str.size()))
        {
            contentLength -= static_cast<int>(str.size());
            // minusContentLength(str.size());
            if (contentLength == 0)
                completion = true;
            entity.push_back(str);
            str.clear();
        }
        else
        {
            entity.push_back(str.substr(0, contentLength));
            str = str.substr(contentLength);
            completion = true;
        }
    }
    else if (entitytype == TRANSFER)
    {
        chunked += str;
        str.clear();
        flag = chunked.find("0\r\n\r\n");
        // flag = str.find("0\r\n");  //talnet 때문에 임시로 대체함
        //에러 발생시 중간에 빠져 나왔을 떄
        if (flag != std::string::npos)
        {
            //chunked 크기 확인하기
            str = chunked.substr(flag + 5);
            chunked = chunked.substr(0, flag);
            // str = str.substr(flag + 3);
            if (chunkedEntity() < 0)
                return (-1);
        }
    }
    return (0);
}

