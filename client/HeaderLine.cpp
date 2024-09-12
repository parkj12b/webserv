/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeaderLine.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 18:56:57 by inghwang          #+#    #+#             */
/*   Updated: 2024/09/07 17:33:33 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HeaderLine.hpp"

//header 줄에서 여러 개의 value를 받을 수 있는 key들을 모음
/*
무시해야 하는 헤더가 있다. 파싱은 진행하나 respond message를 만들 때에는 적용을 하지 않는다. 가끔 브라우저가 사용하기도 한다. 
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
    // v.push_back("cookie");
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
    // v.push_back("content-type");  //원래에는 있어야 하나 cgi쪽에서 잘 안 돼서 없앰
    v.push_back("x-dns-prefetch-control");
    v.push_back("x-forwarded-host");
    v.push_back("x-forwarded-proto");
    v.push_back("x-frame-options");
    v.push_back("x-xss-protection");
    v.push_back("connection");  //upgrade field가 있기는 하나 우리는 허용하지 않는다. 
    return (v);
}

std::vector<std::string>    dateHeaderInit()
{
    std::vector<std::string>    v;

    v.push_back("last-modified");
    v.push_back("if-unmodified-since");
    v.push_back("expires");
    v.push_back("date");
    v.push_back("user-agent");
    v.push_back("retry-after");
    v.push_back("cookie");
    return (v);
}

std::vector<std::string>    vitalHeaderInit()
{
    std::vector<std::string>    v;

    v.push_back("host");
    return (v);
}

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


HeaderLine::HeaderLine() : completion(false), te(NOT), contentType(ENOT), port(0), contentLength(0)
{}

HeaderLine::HeaderLine(const HeaderLine& src) : completion(src.getCompletion()), connect(src.getConnect()), te(src.getTe()), contentType(src.getContentType()), port(src.getPort()), contentLength(src.getContentLength()), key(src.getKey()), value(src.getValue()), header(src.getHeader())
{}

HeaderLine::~HeaderLine()
{
    (void) port;
}

HeaderLine::HeaderLine(int port) : completion(false), connect(true), te(NOT), contentType(ENOT), port(port), contentLength(0)
{}

HeaderLine& HeaderLine::operator=(const HeaderLine& src)
{
    completion = src.getCompletion();
    connect = src.getConnect();
    te = src.getTe();
    contentType = src.getContentType();
    port = src.getPort();
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

bool    HeaderLine::getConnect() const
{
    return (connect);
}

TE  HeaderLine::getTe() const
{
    return (te);
}

CONTENTTYPE HeaderLine::getContentType() const
{
    return (contentType);
}

int HeaderLine::getPort() const
{
    return (port);
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

int HeaderLine::eraseSpace(std::string& str, bool spel)
{
    size_t  pos;

    pos = str.find_first_not_of(' ');
    str.erase(0, pos);
    pos = str.find_last_not_of(' ');
    str.erase(pos + 1);
    if (!str.empty() && spel)
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
        if (header[key].size() || eraseSpace(value, false))
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
        return (1);
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
                break ;
            return (1);
        }
        bracket2 = value.find(')');
        if (bracket2 == std::string::npos)
            return (1);
        if (bracket2 < bracket1)
            return (1);
        value = value.substr(0, bracket1) + value.substr(bracket2 + 1);
    }
    return (0);
}

int HeaderLine::parseTrailer(std::string &temp)
{
    size_t      colon;
    std::string trailerHeader;

    if (eraseSpace(temp, false))
        return (400);
    colon = temp.find(':');
    if (colon != std::string::npos)
    {
        key = temp.substr(0, colon);
        if (eraseSpace(key, true))
            return (400);
        trailerHeader = header["trailer"].front();
        eraseSpace(trailerHeader, true);
        if (key != trailerHeader)
            return (400);
        header["trailer"].pop_front();
        value = temp.substr(colon + 1);
        if (eraseSpace(value, false) || commentDelete() || pushValue())
            return (400);
    }
    else
        return (400);
    return (0);
}

int HeaderLine::makeHeader(std::string& temp)
{
    size_t      colon;

    if (eraseSpace(temp, false))
        return (400);
    colon = temp.find(':');
    if (colon != std::string::npos)
    {
        key = temp.substr(0, colon);
        if (eraseSpace(key, true))
            return (400);
        value = temp.substr(colon + 1);
        if (eraseSpace(value, false) || commentDelete() || pushValue())
            return (400);
    }
    else
        return (400);
    return (0);
}

int HeaderLine::headerError()
{
    std::vector<std::string>::iterator                          itv;
    std::map<std::string, std::deque<std::string> >::iterator   itm;
    std::string                                                 typeStr;
    size_t                                                      pos;

    for (itv = vitalHeader.begin(); itv != vitalHeader.end(); itv++)
    {
        itm = header.find(*itv);
        if (itm == header.end())
            return (400);
    }
    itm = header.find("content-type");
    if (itm != header.end())
    {
        LOG(cout << "cotnent-type" << endl);
        typeStr = header["content-type"].front();
        pos = typeStr.find(";");
        if (pos != std::string::npos)
        {
            header["content-type"].pop_front();
            LOG(cout << "cotnent-type: " << typeStr.substr(0, pos) << endl);
            header["content-type"].push_back(typeStr.substr(0, pos));
            pos = typeStr.find("boundary=");
            if (pos != std::string::npos)
            {
                LOG(cout << "boundary: " << typeStr.substr(pos + 9) << endl);
                header["content-type"].push_back(typeStr.substr(pos + 9));
            }
        }
    }
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
            contentType = CONTENT;
        }
        else
            return (400);
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
    itm = header.find("connection");
    if (itm != header.end())
    {
        if (itm->second.front() == "close")
            connect = false;
    }
    completion = true;
    // itm = header.find("expect");
    // if (itm != header.end())
    // {
    //     if (itm->second.front() != "100-continue")
    //         return (417);
    //     else if (contentType == ENOT)
    //         return (400);
    //     completion = true;
    //     return (100);
    // }
    return (0);
}
