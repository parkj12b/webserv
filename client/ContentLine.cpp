/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ContentLine.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inghwang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 16:24:10 by inghwang          #+#    #+#             */
/*   Updated: 2024/07/20 16:24:12 by inghwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ContentLine.hpp"

extern int logs;

ContentLine::ContentLine() : completion(false), port(0), contentLength(0), maxSize(0)
{}

ContentLine::ContentLine(const ContentLine& src) : completion(src.getCompletion()), contentType(src.getContentType()), port(src.getPort()), contentLength(src.getContentLength()), maxSize(src.getMaxSize()), chunked(src.getChunked()), content(src.getContent())
{}

ContentLine::~ContentLine()
{}

ContentLine::ContentLine(int port) : completion(false), port(port), contentLength(0), maxSize(0)
{}

ContentLine& ContentLine::operator=(const ContentLine& src)
{
    completion = src.getCompletion();
    contentType = src.getContentType();
    port = src.getPort();
    contentLength = src.getContentLength();
    chunked = src.getChunked();
    content = src.getContent();
    return (*this);
}

bool    ContentLine::getCompletion() const
{
    return (completion);
}

CONTENTTYPE ContentLine::getContentType() const
{
    return (contentType);
}

int ContentLine::getContentLength() const
{
    return (contentLength);
}

int ContentLine::getPort() const
{
    return (port);
}

size_t  ContentLine::getMaxSize() const
{
    return (maxSize);
}

std::string ContentLine::getChunked() const
{
    return (chunked);
}

std::vector<std::string>    ContentLine::getContent() const
{
    return (content);
}

void    ContentLine::initContentLine(int initLength, CONTENTTYPE initType)
{
    contentLength = initLength;
    contentType = initType;
}

int ContentLine::chunkedEntity()
{
    std::istringstream  chunkedStream(chunked);
    std::string         temp;
    size_t              ans;
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
        {
            try
            {
                size = std::stoi(temp, nullptr, 16);
            }
            catch(const std::exception& e)
            {
                return (1);
            }
            if (size < 0)
                return (1);
        }
        else
        {
            if (size != static_cast<int>(temp.size()))
                return (2);
            content.push_back(temp);
        }
        ans++;
    }
    completion = true;
    return (0);
}

int ContentLine::makeContentLine(std::string &str, int &status)
{
    size_t  flag;

    maxSize += str.size();
    if (maxSize > 800000000000)
    {
        status = 413;
        return (-1);
    }
    if (contentType == CONTENT)
    {
        std::cout<<contentLength<<' '<<str.size()<<std::endl;
        if (contentLength >= static_cast<int>(str.size()))
        {
            contentLength -= static_cast<int>(str.size());
            if (contentLength == 0)
                completion = true;
            content.push_back(str);
            str.clear();
        }
        else
        {
            content.push_back(str.substr(0, contentLength));
            str = str.substr(contentLength);
            completion = true;
        }
    }
    else if (contentType == TRANSFER)
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
            {
                status = 400;
                return (-1);
            }
        }
    }
    return (0);
}

