/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inghwang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 18:56:57 by inghwang          #+#    #+#             */
/*   Updated: 2024/07/17 18:56:59 by inghwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HeaderLine.hpp"

extern int logs;

//header 줄에서 여러 개의 value를 받을 수 있는 key들을 모음
std::vector<std::string>    manyHeaderInit()
{
    std::vector<std::string>    v;

    v.push_back("accept");
    v.push_back("accept-encoding");
    v.push_back("accept-language");
    v.push_back("sec-ch-ua");
    v.push_back("trailer");
    v.push_back("content-type");
    return (v);
}

//꼭 필요한 헤더들
std::vector<std::string>    vitalHeaderInit()
{
    std::vector<std::string>    v;

    v.push_back("host");
    return (v);
}

//comment가 있을 수 있는 헤더들
std::vector<std::string>    commentHeaderInit()
{
    std::vector<std::string>    v;

    v.push_back("server");
    v.push_back("user-agent");
    v.push_back("via");
    v.push_back("comment");
    return (v);
}

std::vector<std::string> HeaderLine::manyHeader = manyHeaderInit();
std::vector<std::string> HeaderLine::vitalHeader = vitalHeaderInit();
std::vector<std::string> HeaderLine::commentHeader = commentHeaderInit();

void    HeaderLine::eraseSpace(std::string& str, bool space)
{
    size_t  pos;

    pos = str.find_first_not_of(' ');
    str.erase(0, pos);
    pos = str.find_last_not_of(' ');
    str.erase(pos + 1);
    if (space)
    {
        for (std::string::iterator it = str.begin(); it != str.end(); it++)
            *it = std::tolower(*it);
    }
}

// bool    HeaderLine::checkMime(std::string temp)
// {
//     std::istringstream  strStream(temp);
//     std::string         str;
//     int                 ans;

//     ans = 0;
//     while (std::getline(strStream, str, '/'))
//         ans++;
//     if (ans == 2)
//         return (true);
//     return (false);
// }

int HeaderLine::pushValue()
{
    std::vector<std::string>::iterator  it;
    std::string                         str;
    std::string                         answer;
    bool                                comma;

    comma = true;
    for (std::string::iterator it = value.begin(); it != value.end(); it++)
    {
        if (*it == '"')
            comma = !comma;
        else if (comma && *it == ',')
            answer += '\0';
        else
            answer += *it;
    }
    if (!comma)
        return (-1);
    std::istringstream  strStream(answer);
    comma = true;
    while (std::getline(strStream, str, '\0'))
    {
        eraseSpace(str, false);
        if (str.empty())
            continue ;
        header[key].push_back(str);
        comma = false;
    }
    if (comma) //, , ,
        return (-2);  //400
    it = std::find(manyHeader.begin(), manyHeader.end(), key);
    if (it == manyHeader.end())
    {
        if (header[key].size() > 1)
            return (-2);
    }
    return (0);
}

int HeaderLine::commentDelete()
{
    std::vector<std::string>::iterator  it;
    size_t                              bracket1;
    size_t                              bracket2;

    it = std::find(commentHeader.begin(), commentHeader.end(), key);
    if (it == commentHeader.end())
        return (0);
    while (1)
    {
        bracket1 = value.find('(');
        if (bracket1 == std::string::npos)
        {
            bracket2 = value.find(')');
            if (bracket2 == std::string::npos)
                return (0);
            else
                return (-1);
        }
        bracket2 = value.find(')');
        if (bracket2 == std::string::npos)
            return (-1);
        value = value.substr(0, bracket1) + value.substr(bracket2 + 1);
    }
}

HeaderLine::HeaderLine() : completion(false), te(NOT), entitytype(ENOT)
{}

HeaderLine::HeaderLine(const HeaderLine& src)
{
    completion = src.getCompletion();
    te = src.getTe();
    entitytype = src.getEntitytype();
    contentLength = src.getContentLength();
    key = src.getKey();
    value = src.getValue();
    header = src.getHeader();
}

HeaderLine::~HeaderLine() {}

HeaderLine& HeaderLine::operator=(const HeaderLine& src)
{
    entitytype = src.getEntitytype();
    te = src.getTe();
    completion = src.getCompletion();
    contentLength = src.getContentLength();
    key = src.getKey();
    value = src.getValue();
    header = src.getHeader();
    return (*this);
}

bool    HeaderLine::getCompletion() const
{
    return (completion);
}

TE  HeaderLine::getTe() const
{
    return (te);
}

ENTITYTYPE  HeaderLine::getEntitytype() const
{
    return (entitytype);
}

int HeaderLine::getContentLength() const
{
    return (contentLength);
}

std::string HeaderLine::getKey() const
{
    return (key);
}

std::string HeaderLine::getValue() const
{
    return (value);
}
std::map<std::string, std::deque<std::string> > HeaderLine::getHeader() const
{
    return (header);
}

void    HeaderLine::setContentLength(int minus)
{
    contentLength -= minus;
}

void    HeaderLine::setTrailer(TE temp)
{
    te = temp;
}

int HeaderLine::checkTrailer(std::string &temp)
{
    size_t      colon;
    std::string trailerHeader;

    eraseSpace(temp, false);
    if (temp.empty())
        return (-1);  //400
    //pop_front(): 앞쪽에서 요소를 제거합니다.
    //front(): 앞쪽 요소를 반환합니다.
    colon = temp.find(':');
    if (colon != std::string::npos)
    {
        key = temp.substr(0, colon);
        eraseSpace(key, true);
        if (key.empty())
            return (-2);  //400
        trailerHeader = header["trailer"].front();
        eraseSpace(trailerHeader, true);
        if (key != trailerHeader)
            return (-3);  //400
        header["trailer"].pop_front();
        value = temp.substr(colon + 1);
        eraseSpace(value, false);
        if (value.empty())
            return (-2);  //400
        if (commentDelete() < 0)
            return (-2);  //400
        // std::cout<<"key: "<<key;
        if (pushValue() < 0)
            return (-3);  //400
        // header[key].push_back(str);
        if (header["trailer"].empty())
            return (0);
    }
    else
    {
        return (-2);  //400
        // if (key.size() == 0 && !checkMime(temp))
        //     return (-2);  //message/htpp타입이 아닌데 obs-fold를 사용한 상황
        // value = temp;
        // if (pushValue() < 0)
        //     return (-2);
    }
    return (0);
}

int HeaderLine::plus(std::string& temp)
{
    std::string str;
    size_t      colon;

    // std::cout<<temp<<std::endl;
    eraseSpace(temp, false);
    if (temp.empty())
        return (-1);  //400
    // if (temp[temp.size() - 1] == ',')
    //     temp.erase(temp.size() - 1);
    // std::cout<<temp<<std::endl;
    colon = temp.find(':');
    if (colon != std::string::npos)
    {
        key = temp.substr(0, colon);
        eraseSpace(key, true);
        if (key.empty())
            return (-2);  //400
        value = temp.substr(colon + 1);
        eraseSpace(value, false);
        if (value.empty())
            return (-3);  //400
        // std::cout<<"key: "<<key;
        if (commentDelete() < 0)
            return (-2);  //400
        if (pushValue() < 0)
            return (-4);  //400
        // header[key].push_back(str);
    }
    else
    {
        return (-5);  //400
        // if (key.size() == 0 && !checkMime(temp))
        //     return (-2);  //message/htpp타입이 아닌데 obs-fold를 사용한 상황
        // value = temp;
        // if (pushValue() < 0)
        //     return (-2);
    }
    return (0);
}

int HeaderLine::headerError()
{
    std::vector<std::string>::iterator                          itv;
    std::map<std::string, std::deque<std::string> >::iterator   itm;

    for (itv = vitalHeader.begin(); itv != vitalHeader.end(); itv++)
    {
        itm = header.find(*itv);
        if (itm == header.end())
        {
            return (-1);  //400
        }
    }
    itm = header.find("content-type");
    if (itm != header.end())
    {
        while (itm->second.size() > 1)
            itm->second.pop_front();
    }
    itm = header.find("content-length");
    if (itm != header.end())
    {
        itm = header.find("transfer-encoding");
        if (itm == header.end())
        {
            contentLength = std::stoi(header["content-length"].front());
            if (contentLength < 0)
            {
                return (-2);
            }
            entitytype = CONTENT;
        }
        else
        {
            return (-2);  //400
        }
    }
    else
    {
        itm = header.find("transfer-encoding");
        if (itm != header.end())
        {
            if (header["transfer-encoding"].front() != "chunked")
            {
                std::cout<<"good\n"<<std::endl;
                return (-2);
            }
            entitytype = TRANSFER;
        }
    }
    itm = header.find("trailer");
    if (itm != header.end())
        te = YES;
    completion = true;
    return (0);
}
