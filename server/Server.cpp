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

void    Server::errorHandler(std::string message)
{
    std::cerr<<message<<"\n";
    exit(1);
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
    int                 clntFd;
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
    const char *temp = "HTTP/1.1 200 OK\nContent-Type: text/html;charset=UTF-8\nContent-Length: ";

    count = kevent(kq, &fdList[0], fdList.size(), store, EVENTCNT, NULL);
    while ((count = kevent(kq, &fdList[0], fdList.size(), store, EVENTCNT, NULL)) < 0);
    fdList.clear();
    for (int i = 0; i < count; i++)
    {
        if (static_cast<int>(store[i].ident) == serverFd)
        {
            // if (store[i].flags == EV_ERROR)
            //     continue ;
                // errorHandler("server error.");
            if (store[i].filter == EVFILT_READ)
                plusClient();
        }
        else
        {
            // if (store[i].flags == EV_ERROR)
            //     continue ;
                // errorHandler("client error.");
            if (store[i].filter == EVFILT_READ)
            {
                // std::cout<<"==EVFILT_READ==\n";
                while ((readSize = read(store[i].ident, buffer, BUFFER_SIZE)) < 0);
                // if (readSize < 0)
                //     errorHandler("clientFd's read error");
                buffer[readSize] = '\0';
                //여기서 적어주어야 함
                client[store[i].ident].setMessage(buffer);
                // write(1, buffer, readSize);
                if (client[store[i].ident].getRequestFin() == true)
                {
                    EV_SET(&store[i], store[i].ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                    plusEvent(store[i].ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
                    client[store[i].ident].showMessage();
                    // client[store[i].ident].setTemp();
                }
            }
            else if (store[i].filter == EVFILT_WRITE)
            {
                //write부분 고치기
                //write의 성공 및 실패 여부에 따라 바뀌게 짜는 것이 좋을 듯하다. 
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
                EV_SET(&store[i], store[i].ident, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
                close(fd);
                std::cout<<"read delete\n";
                client.erase(store[i].ident);
                close(static_cast<int>(store[i].ident));
            }
        }
    }
}

