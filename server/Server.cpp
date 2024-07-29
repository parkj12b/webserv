/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inghwang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 10:56:52 by inghwang          #+#    #+#             */
/*   Updated: 2024/07/08 10:56:53 by inghwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

extern int logs;

std::map<int, std::string>  statusContentInit()
{
    std::map<int, std::string>  m;

    m[100] = "Continue";
    m[101] = "Switching Protocols";
    m[102] = "Processing";
    m[103] = "Early Hints";
    m[200] = "OK";
    m[201] = "Created";
    m[202] = "Accepted";
    m[203] = "Non-Authoritative Information";
    m[204] = "No Content";
    m[205] = "Reset Content";
    m[206] = "Partial Content";
    m[207] = "Multi-Status";
    m[208] = "Already Reported";
    m[214] = "Transformation Applied";
    m[226] = "IM Used";
    m[300] = "Multiple Choices";
    m[301] = "Moved Permanently";
    m[302] = "Found";
    m[303] = "See Other";
    m[304] = "Not Modified";
    m[305] = "Use Proxy";
    m[307] = "Temporary Redirect";
    m[308] = "Permanent Redirect";
    m[400] = "Bad Request";
    m[401] = "Unauthorized";
    m[402] = "Payment Required";
    m[403] = "Forbidden";
    m[404] = "Not Found";
    m[405] = "Method Not Allowed";
    m[406] = "Not Acceptable";
    m[407] = "Proxy Authentication Required";
    m[408] = "Request Timeout";
    m[409] = "Conflict";
    m[410] = "Gone";
    m[411] = "Length Required";
    m[412] = "Precondition Failed";
    m[413] = "Payload Too Large";
    m[414] = "Request-URI Too Long";
    m[415] = "Unsupporred Media Type";
    m[416] = "Request Range Not Staisfiable";
    m[417] = "Expectation Failed";
    m[418] = "I’m a teapot";
    m[420] = "Enhance Your Calm";
    m[421] = "Misdirected Request";
    m[422] = "Unprocessable Entity";
    m[423] = "Locked";
    m[424] = "Failed Dependency";
    m[425] = "Too Early";
    m[426] = "Upgrade Required";
    m[428] = "Precondition Required";
    m[429] = "Too many Requests";
    m[431] = "Request Header Fields Too Large";
    m[444] = "No Response";
    m[450] = "Blocked by Windows Parental Controls";
    m[451] = "Unavailable For Legal Reasons";
    m[497] = "HTTP Request Sent to HTTPS Port";
    m[498] = "Token expired/invalid";
    m[499] = "Client Closed Request";
    m[500] = "Internal Server Error";
    m[501] = "Not Implemented";
    m[502] = "Bad Gateway";
    m[503] = "Service Unavailable";
    m[504] = "Gateway Timeout";
    m[506] = "Variant Also Negotiates";
    m[507] = "Insufficient Storage";
    m[508] = "Loop Detected";
    m[509] = "Bandwidth Limit Exceeded";
    m[510] = "Not Extended";
    m[511] = "Network Authentication Required";
    m[521] = "Web Server Is Down";
    m[522] = "Connection Timed Out";
    m[523] = "Origin Is Unreachable";
    m[525] = "SSL Handshake Failed";
    m[530] = "Site Frozen";
    m[599] = "Network Connect Timeout Error";
    return (m);
}

std::map<int, std::string>  Server::statusContent = statusContentInit();

Server::Server()
{}

Server::Server(int fd)
{
    serverFd = fd;
}

Server::Server(const Server& src)
{
    this->serverFd = src.getServerFd();
    this->client = src.getClient();
}

Server&  Server::operator=(const Server& src)
{
    this->serverFd = src.getServerFd();
    this->client = src.getClient();
    return (*this);
}

Server::~Server()
{}

int Server::getServerFd(void) const
{
    return (serverFd);
}

std::map<int, Client>  Server::getClient(void) const
{
    return (client);
}


int Server::plusClient(void)
{
    int           clntFd;
    struct sockaddr_in  clntAdr;
    socklen_t           adrSize;

    adrSize = sizeof(clntAdr);
    //accept 무한 루프
    while ((clntFd = accept(serverFd, (struct sockaddr *)&clntAdr, &adrSize)) < 0);
    // if (clntFd < 0)
    //     errorHandler("accept error.");
    client[clntFd] = Client(clntFd);
    std::cout<<"temp delete\n";
    return (clntFd);
    // plusEvent(clntFd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
    //나갈 때 소멸자가 호출됨
}

EVENT Server::clientRead(int clientFd)
{
    //buffer 문제인지 생각해보기
    int     readSize;

    if (client[clientFd].getRequestStatus() > 0)
        return (ING);
    std::cout<<"ddddddddread eventdddddddd"<<std::endl;
    readSize = read(clientFd, buffer, BUFFER_SIZE);
    if (readSize <= 0)
    {
        std::cout<<"read error\n";
        return (ERROR);  //client 연결 종료 시키기
    }
    std::cout<<readSize<<std::endl;
    buffer[readSize] = '\0';
    client[clientFd].setMessage(buffer);
    if (client[clientFd].getRequestFin() == true || client[clientFd].getRequestStatus() > 0)
    {
        // request 완성 -> respond 만들면 되지 않나?
        if (client[clientFd].getRequestFin() == true)
            client[clientFd].showMessage();
        return (FINISH);
    }
    // if (readSize < BUFFER_SIZE)
    // {
    //     client[clientFd].setRequestStatus(400);
    //     return (FINISH);
    // }
    return (ING);
}

EVENT Server::clientWrite(int clientFd)
{
    int     sum;
    int     readSize;
    int     fd;
    char    buffer[BUFFER_SIZE];
    std::string str;
    const char  *temp = "HTTP/1.1 200 OK\nContent-Type: text/html;charset=UTF-8\nContent-Length: ";

    if (client[clientFd].getRequestStatus() > 0)
        errorHandler(client[clientFd]);
    else
    {
        fd = open("./index.html", O_RDONLY);
        sum = 0;
        while (1)
        {
            readSize = read(fd, buffer, BUFFER_SIZE);
            if (readSize <= 0)
                break;
            sum += readSize;
        }
        close(fd);
        write(clientFd, temp, strlen(temp));
        str = std::to_string(sum);
        write(clientFd, str.c_str(), str.size());
        write(clientFd, "\n\n", 2);
        fd = open("./index.html", O_RDONLY);
        while (1)
        {
            readSize = read(fd, buffer, BUFFER_SIZE);
            if (readSize <= 0)
                break;
            write(clientFd, buffer, strlen(buffer));
        }
        close(fd);
        close(static_cast<int>(clientFd));
        client.erase(clientFd);
    }
    std::cout<<"read delete\n";
    return (FINISH);
}

void    Server::errorHandler(Client& c)
{
    std::string temp;
    int         readSize;
    char        buffer[BUFFER_SIZE];
    int         fd;
    int         sum;

    //시작줄
    write(c.getFd(), "HTTP/1.1 ", 9);
    temp = std::to_string(c.getRequestStatus());
    temp += " " + statusContent[c.getRequestStatus()];
    write(c.getFd(), temp.c_str(), temp.size());
    //헤더줄
    write(c.getFd(), "\nContent-Type: text/html;charset=UTF-8\nContent-Length: ", 55);
    sum = 0;
    fd = open("./error.html", O_RDONLY);
    while (1)
    {
        readSize = read(fd, buffer, BUFFER_SIZE);
        if (readSize <= 0)
            break;
        sum += readSize;
    }
    close(fd);
    temp = std::to_string(sum);
    write(c.getFd(), temp.c_str(), temp.size());
    write(c.getFd(), "\n\n", 2);
    //본문
    fd = open("./error.html", O_RDONLY);
    while (1)
    {
        readSize = read(fd, buffer, BUFFER_SIZE);
        if (readSize <= 0)
            break;
        write(c.getFd(), buffer, strlen(buffer));
    }
    close(fd);
    close(c.getFd());
    client.erase(c.getFd());
}

void    Server::clientError(int clientFd)
{
    close(clientFd);
    client.erase(clientFd);
}










