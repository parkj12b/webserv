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
{
    struct sockaddr_in  serverAdr;
    int                 option;

    //IP config file에서 받기
    while ((serverFd = socket(PF_INET, SOCK_STREAM, 0)) < 0);
    // serverFd = socket(PF_INET, SOCK_STREAM, 0);
    // if (serverFd < 0)
    //     errorHandler("socket error.");
    option = 1;
    while (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)));
    // do {
    //     setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    // } while (errno != 0);
    memset(&serverAdr, 0, sizeof(serverAdr));
    // nullSet(&serverAdr, sizeof(serverAdr));
    serverAdr.sin_family = AF_INET;
    serverAdr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAdr.sin_port = htons(PORT);
    while (bind(serverFd, (struct sockaddr *)&serverAdr, sizeof(serverAdr)) < 0);
    // if (bind(serverFd, (struct sockaddr *)&serverAdr, sizeof(serverAdr)) < 0)
    //     errorHandler("bind error.");
    while (listen(serverFd, CLIENT_CNT) < 0);
    // if (listen(serverFd, CLIENT_CNT) < 0) 
    //     errorHandler("listen error.");
    while ((kq = kqueue()) < 0);
    // kq = kqueue();
    // if (kq < 0)
    //     errorHandler("kqueue error.");
    plusEvent(serverFd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
}

Server::Server(const Server& src)
{
    this->fdList = src.getFdList();
    this->respond = src.getRespond();
    this->serverFd = src.getServerFd();
    this->kq = src.getKq();
    this->client = src.getClient();
}

Server&  Server::operator=(const Server& src)
{
    this->fdList = src.getFdList();
    this->respond = src.getRespond();
    this->serverFd = src.getServerFd();
    this->kq = src.getKq();
    this->client = src.getClient();
    return (*this);
}

Server::~Server()
{}

std::vector<struct kevent>  Server::getFdList(void) const
{
    return (fdList);
}

std::map<std::string, std::string>  Server::getRespond(void) const
{
    return (respond);
}

int Server::getServerFd(void) const
{
    return (serverFd);
}

int Server::getKq(void) const
{
    return (kq);
}

std::map<int, Client>  Server::getClient(void) const
{
    return (client);
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
    client.erase(c.getFd());
    close(c.getFd());
}

void    Server::nullSet(void *ptr, size_t len)
{
    unsigned char   *zeroSet;
    size_t          i;

    zeroSet = (unsigned char *)ptr;
    i = 0;
    while (i < len)
        zeroSet[i++] = '\0';
}

void    Server::plusEvent(uintptr_t fd, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata)
{
    struct kevent   temp;

    EV_SET(&temp, fd, filter, flags, fflags, data, udata);
    fdList.push_back(temp);  //temp를 복사해서 저장을 함
}

void    Server::plusClient(void)
{
    uintptr_t           clntFd;
    struct sockaddr_in  clntAdr;
    socklen_t           adrSize;
    Client              newClient;

    adrSize = sizeof(clntAdr);
    //accept 무한 루프
    while ((clntFd = accept(serverFd, (struct sockaddr *)&clntAdr, &adrSize)) < 0);
    // if (clntFd < 0)
    //     errorHandler("accept error.");
    plusEvent(clntFd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
    newClient.setFd(clntFd);  //요기서 생성이 한 번 되고
    client[clntFd] = newClient;
    //나갈 때 소멸자가 호출됨
    std::cout<<"temp delete\n";
}

void    Server::mainLoop(void)
{
    struct kevent   store[EVENTCNT];
    char            buffer[BUFFER_SIZE];
    std::string     str;
    int             count;
    int             readSize;
    int             sum = 0;
    const char  *temp = "HTTP/1.1 200 OK\nContent-Type: text/html;charset=UTF-8\nContent-Length: ";

    // count = kevent(kq, &fdList[0], fdList.size(), store, EVENTCNT, NULL);
    while ((count = kevent(kq, &fdList[0], fdList.size(), store, EVENTCNT, NULL)) < 0);
    fdList.clear();
    for (int i = 0; i < count; i++)
    {
        if (static_cast<int>(store[i].ident) == serverFd)
        {
            if (store[i].flags == EV_ERROR)
                continue ;   // 연결된 모든 client 연결 종료 시키기
                // errorHandler("server error.");
            if (store[i].filter == EVFILT_READ)
                plusClient();
        }
        else
        {
            if (store[i].flags == EV_ERROR)
                continue ;  // client 연결 종료시키기
                // errorHandler("client error.");
            if (store[i].filter == EVFILT_READ)
            {
                readSize = read(store[i].ident, buffer, BUFFER_SIZE);
                if (readSize <= 0)
                    continue ;  //client 연결 종료 시키기
                buffer[readSize] = '\0';
                std::cout<<buffer;
                client[store[i].ident].setMessage(buffer);
                if (client[store[i].ident].getRequestFin() == true || client[store[i].ident].getRequestStatus() > 0)
                {
                    // request 완성 -> respond 만들면 되지 않나?
                    EV_SET(&store[i], store[i].ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                    plusEvent(store[i].ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
                    if (client[store[i].ident].getRequestFin() == true)
                        client[store[i].ident].showMessage();
                }
            }
            else if (store[i].filter == EVFILT_WRITE)
            {
                //write부분 고치기
                //write의 성공 및 실패 여부에 따라 바뀌게 짜는 것이 좋을 듯하다. 
                if (client[store[i].ident].getRequestStatus() > 0)
                    errorHandler(client[store[i].ident]);
                else
                {
                    int fd = open("./index.html", O_RDONLY);
                    sum = 0;
                    while (1)
                    {
                        readSize = read(fd, buffer, BUFFER_SIZE);
                        if (readSize <= 0)
                            break;
                        sum += readSize;
                    }
                    close(fd);
                    write(store[i].ident, temp, strlen(temp));
                    str = std::to_string(sum);
                    write(store[i].ident, str.c_str(), str.size());
                    write(store[i].ident, "\n\n", 2);
                    fd = open("./index.html", O_RDONLY);
                    while (1)
                    {
                        readSize = read(fd, buffer, BUFFER_SIZE);
                        if (readSize <= 0)
                            break;
                        write(store[i].ident, buffer, strlen(buffer));
                    }
                    close(fd);
                }
                EV_SET(&store[i], store[i].ident, EVFILT_WRITE, EV_DELETE, 0, 0,NULL);
                std::cout<<"read delete\n";
                close(static_cast<int>(store[i].ident));
                client.erase(store[i].ident);
            }
        }
    }
}

