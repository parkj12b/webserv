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
/*
무시해야 하는 헤더가 있다. 파싱은 진행하나 respond message를 만들 때에는 적용을 하지 않는다. 
accept-charset
Content-DPR
Digest
DNT
DPR
Pragma
Sec-CH-UA-Full-Version
Tk
Viewport-Width
Want-Digest
Warning
Width
*/

/*
Content-Disposition헤더를 보니 큰 따옴표를 넣어주고 나중에 respond할 때에 처리해주는 방향으로 가는 것이 조금더 좋을 것 같다는 느낌이 살짝듬
*/
/*
날짜 value field
date, expires, last-modified, if-modified-since, retry-after
*/

std::vector<std::string>    singleHeaderInit()
{
    std::vector<std::string>    v;

    v.push_back("accept-ranges");
    v.push_back("access-control-allow-credentials");
    v.push_back("access-control-allow-origin");
    v.push_back("access-control-max-age");
    v.push_back("age");
    v.push_back("alt-used");
    v.push_back("attribution-reporting-eligible");
    v.push_back("attribution-reporting-register-source");
    v.push_back("attribution-reporting-register-trigger");
    v.push_back("content-digest");
    v.push_back("content-disposition");
    v.push_back("content-length");
    v.push_back("content-location");
    v.push_back("content-range");
    v.push_back("content-security-policy");
    v.push_back("content-security-policy-report-only");
    v.push_back("cookie");
    v.push_back("cross-origin-embedder-policy");
    v.push_back("cross-origin-opener-policy");
    v.push_back("cross-origin-resource-policy");
    v.push_back("device-memory");
    v.push_back("downlink");
    v.push_back("early-data");
    v.push_back("ect");
    v.push_back("etag");
    v.push_back("expect");
    v.push_back("forwarded");
    v.push_back("from");
    v.push_back("host");
    v.push_back("location");
    v.push_back("max-forwards");
    v.push_back("origin");
    v.push_back("origin-agent-cluster");
    v.push_back("referer");
    v.push_back("retry-after");
    v.push_back("rtt");
    v.push_back("save-data");
    v.push_back("sec-ch-prefers-color-scheme");
    v.push_back("sec-ch-prefers-reduced-motion");
    v.push_back("sec-ch-prefers-reduced-transparency");
    v.push_back("sec-ch-ua-arch");
    v.push_back("sec-ch-ua-bitness");
    v.push_back("sec-ch-ua-mobile");
    v.push_back("sec-ch-ua-model");
    v.push_back("sec-ch-ua-platform");
    v.push_back("sec-ch-ua-platform-version");
    v.push_back("sec-fetch-dest");
    v.push_back("sec-fetch-mode");
    v.push_back("sec-fetch-site");
    v.push_back("sec-fetch-user");
    v.push_back("sec-gpc");
    v.push_back("sec-purpose");
    v.push_back("sec-websocket-accept");
    v.push_back("server");
    v.push_back("service-worker-navigation-preload");
    v.push_back("strict-transport-security");
    v.push_back("timing-allow-origin");
    v.push_back("transfer-encoding");
    v.push_back("upgrade-insecure-requests");
    v.push_back("x-content-type-options");
    v.push_back("content-type");
    /*
    content-type은 논란이 많다. content가 들어오는 형식이 어떠한지를 결정해주는 헤더이다. 
    다만 이 헤더가 여러개 들어올 경우에 가장 마지막의 헤더를 받아야 한다는 말이 rfc에 존재한다.
    또 rfc에서 이를 서로 다른 구현 방식 때문에 오류 처리 방식이 다를 수도 있다고 나와있다. 
    현재로써는 물론 여기서 마지막의 것을 처리해주는 방식이 더 적절할 수도 있다. 
    하지만 후에 프록시와 게이트웨이에서 잘못 처리할 수도 있고(물론 이런 경우는 극히 드믈거나 없을 것이다.),
    클라이언트에 따라 처리하는 content-type을 처리하는 방식이 다를 수 있을 것이라고 생각이 들기도 한다. 
    따라서 우리는 이것이 여러 개 들어올 경우 에러를 처리하기로 했다. 
    */
    v.push_back("x-dns-prefetch-control");
    v.push_back("x-forwarded-host");
    v.push_back("x-forwarded-proto");
    v.push_back("x-frame-options");
    v.push_back("x-xss-protection");
    return (v);
}

std::vector<std::string>    dateHeaderInit()
{
    std::vector<std::string>    v;

    v.push_back("last-modified");  //,(쉼표)처리를 어떻게 진행하면 좋을지 생각해야 할듯
    v.push_back("if-unmodified-since");  //,(쉼표)처리를 어떻게 진행하면 좋을지 생각해야 할듯
    v.push_back("expires");  //date와 같은 날짜를 받음
    v.push_back("date");  //,(쉼표)처리를 어떻게 진행하면 좋을지 생각해야 할듯
    v.push_back("user-agent");  //괄호 밖에 쉼표가 올수도 있다. 그경우는 보지 못하고 chatgpt도 생성 못함
    v.push_back("retry-after");
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

std::vector<std::string> HeaderLine::singleHeader = singleHeaderInit();
std::vector<std::string> HeaderLine::dateHeader = dateHeaderInit();
std::vector<std::string> HeaderLine::vitalHeader = vitalHeaderInit();
std::vector<std::string> HeaderLine::commentHeader = commentHeaderInit();

int HeaderLine::eraseSpace(std::string& str, bool space)
{
    size_t  pos;

    pos = str.find_first_not_of(' ');
    str.erase(0, pos);
    pos = str.find_last_not_of(' ');
    str.erase(pos + 1);
    if (!str.empty() && space)
    {
        for (std::string::iterator it = str.begin(); it != str.end(); it++)
            *it = std::tolower(*it);
    }
    if (str.empty())
        return (1);
    return (0);
}

int HeaderLine::pushValue()
{
    std::vector<std::string>::iterator  it;
    std::string                         str;
    std::string                         answer;
    bool                                comma;

    it = std::find(dateHeader.begin(), dateHeader.end(), key);
    if (it != dateHeader.end())
    {
        if (header[key].size())
            return (1);
        if (eraseSpace(value, false))
            return (1);
        header[key].push_back(value);
        return (0);
    }
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
        return (1);
    std::istringstream  strStream(answer);
    comma = true;
    while (std::getline(strStream, str, '\0'))
    {
        if (eraseSpace(str, false))
            continue ;
        header[key].push_back(str);
        comma = false;
    }
    if (comma) //, , ,
        return (1);  //400
    it = std::find(singleHeader.begin(), singleHeader.end(), key);
    if (it != singleHeader.end())
    {
        if (header[key].size() > 1)
            return (1);
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
                return (1);
        }
        bracket2 = value.find(')');
        if (bracket2 == std::string::npos)
            return (1);
        if (bracket2 > bracket1)
            return (1);
        value = value.substr(0, bracket1) + value.substr(bracket2 + 1);
    }
    return (0);
}

HeaderLine::HeaderLine() : completion(false), te(NOT), contentType(ENOT)
{}

HeaderLine::HeaderLine(const HeaderLine& src)
{
    completion = src.getCompletion();
    te = src.getTe();
    contentType = src.getContentType();
    contentLength = src.getContentLength();
    key = src.getKey();
    value = src.getValue();
    header = src.getHeader();
}

HeaderLine::~HeaderLine() {}

HeaderLine& HeaderLine::operator=(const HeaderLine& src)
{
    contentType = src.getContentType();
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

CONTENTTYPE HeaderLine::getContentType() const
{
    return (contentType);
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

    if (eraseSpace(temp, false))
        return (400);  //400
    //pop_front(): 앞쪽에서 요소를 제거합니다.
    //front(): 앞쪽 요소를 반환합니다.
    colon = temp.find(':');
    if (colon != std::string::npos)
    {
        key = temp.substr(0, colon);
        if (eraseSpace(key, true))
            return (400);  //400
        trailerHeader = header["trailer"].front();
        eraseSpace(trailerHeader, true);
        if (key != trailerHeader)
            return (400);  //400
        header["trailer"].pop_front();
        value = temp.substr(colon + 1);
        if (eraseSpace(value, false))
            return (400);  //400
        if (commentDelete())
            return (400);  //400
        // std::cout<<"key: "<<key;
        if (pushValue())
            return (400);  //400
        // header[key].push_back(str);
    }
    else
    {
        return (400);  //400
    }
    return (0);
}

int HeaderLine::plus(std::string& temp)
{
    std::string str;
    size_t      colon;

    // std::cout<<temp<<std::endl;
    if (eraseSpace(temp, false))
        return (400);
    // if (temp[temp.size() - 1] == ',')
    //     temp.erase(temp.size() - 1);
    // std::cout<<temp<<std::endl;
    colon = temp.find(':');
    if (colon != std::string::npos)
    {
        key = temp.substr(0, colon);
        if (eraseSpace(key, true))
            return (400);
        value = temp.substr(colon + 1);
        if (eraseSpace(value, false))
            return (400);
        // std::cout<<"key: "<<key;
        if (commentDelete())
            return (400);
        if (pushValue())
            return (400);
        // header[key].push_back(str);
    }
    else
    {
        return (400);
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
            return (400);  //400
    }
    // itm = header.find("content-type");
    // if (itm != header.end())
    // {
    //     while (itm->second.size() > 1)
    //         itm->second.pop_front();
    // }
    itm = header.find("content-length");
    if (itm != header.end())
    {
        itm = header.find("transfer-encoding");
        if (itm == header.end())
        {
            std::stringstream   ss(header["content-length"].front());

            ss>>contentLength;
            if (ss.fail() || !ss.eof())
                return (400);
            if (contentLength > 100000000)  //serverConfig에서 받아올 것
                return (400);
            contentType = CONTENT;
        }
        else
            return (400);  //400
    }
    else
    {
        itm = header.find("transfer-encoding");
        if (itm != header.end())
        {
            if (header["transfer-encoding"].front() != "chunked")
                return (400);
            contentType = TRANSFER;
        }
    }
    itm = header.find("trailer");
    if (itm != header.end())
    {
        if (contentType != TRANSFER)
            return (400);
        te = YES;
    }
    itm = header.find("expect");
    if (itm != header.end())
    {
        if (itm->second.front() != "100-continue")
            return (417);
        else if (contentType == ENOT)
            return (408);
        completion = true;
        return (100);
    }
    completion = true;
    return (0);
}
