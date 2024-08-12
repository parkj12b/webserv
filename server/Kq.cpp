/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kq.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:08:58 by inghwang          #+#    #+#             */
/*   Updated: 2024/08/12 13:30:18 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Kq.hpp"

extern int logs;

Kq::Kq()
{
    struct sockaddr_in  serverAdr;
    int                 option;
    int                 serverFd;
    //temp
    int                 portNum[4] = {80, 800, 8000, 8080};

    while ((kq = kqueue()) < 0);
    option = 1;
    //여기서 루프 돌면서 server socket전부다 만들기
    for (int i = 0; i < 4; i++)
    {
        while ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) <= 0);
        while (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0);
        memset(&serverAdr, 0, sizeof(serverAdr));
        serverAdr.sin_family = AF_INET;
        serverAdr.sin_addr.s_addr = htonl(INADDR_ANY);  //ip를 어떻게 가져오는 방향에 대해 고민하기
        serverAdr.sin_port = htons(portNum[i]);   //port도 마찬가지로 어떻게 가져오는지
        while (::bind(serverFd, (struct sockaddr *)&serverAdr, sizeof(serverAdr)) < 0)
        {
            if (errno == EADDRINUSE)
                std::exit(1);
        }
        while (listen(serverFd, CLIENT_CNT) < 0);
        plusEvent(serverFd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
        server[serverFd] = Server(serverFd);
    }
}

Kq::Kq(const Kq& src)
{
    kq = src.getKq();
    fdList = src.getFdList();
    server = src.getServer();
    findServer = src.getFindServer();
}

Kq& Kq::operator=(const Kq& src)
{
    kq = src.getKq();
    fdList = src.getFdList();
    server = src.getServer();
    findServer = src.getFindServer();
    return (*this);
}

Kq::~Kq()
{}

int Kq::getKq() const
{
    return (kq);
}

std::vector<struct kevent>  Kq::getFdList() const
{
    return (fdList);
}

std::map<int, Server>   Kq::getServer() const
{
    return (server);
}

std::map<int, int>  Kq::getFindServer() const
{
    return (findServer);
}

void    Kq::plusEvent(uintptr_t fd, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata)
{
    struct kevent   temp;

    EV_SET(&temp, fd, filter, flags, fflags, data, udata);
    fdList.push_back(temp);  //temp를 복사해서 저장을 함
}

void    Kq::plusClient(int serverFd)
{
    int clientFd;

    clientFd = server[serverFd].plusClient();
    std::cout<<"plus client"<<std::endl;
    plusEvent(clientFd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
    findServer[clientFd] = serverFd;
}

void    Kq::eventRead(struct kevent& store)
{
    int     serverFd;
    EVENT   event;

    if (store.ident == 0)
        return ;
    serverFd = findServer[store.ident];
    if (serverFd == 0)
        return ;
    event = server[serverFd].clientRead(store);
    switch (event)
    {
        case ERROR:
            clientFin(store);
            //연결 종료하기
            break ;
        case ING:
            break ;
        case EXPECT:
            plusEvent(store.ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
            break ;
        case FINISH:
            // std::cout<<"read event delete\n";
            plusEvent(store.ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
            break ;
    }
}

void    Kq::eventWrite(struct kevent& store)
{
    int     serverFd;
    EVENT   event;

    if (store.ident == 0)
        return ;
    serverFd = findServer[store.ident];
    if (serverFd == 0)
        return ;
    event = server[serverFd].clientWrite(store);
    switch (event)
    {
        case ERROR:
            clientFin(store);
            break ;
        case ING:
            break ;
        case FINISH:
            std::cout<<"write delete\n";
            clientFin(store);
            break ;
        case EXPECT:
            EV_SET(&store, store.ident, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
            break ;
    }
}

void    Kq::mainLoop()
{
    struct kevent   store[EVENTCNT];
    int             count;

    while ((count = kevent(kq, &fdList[0], fdList.size(), store, EVENTCNT, NULL)) < 0);
    fdList.clear();
    for (int i = 0; i < count; i++)
    {
        if (server.find(static_cast<int>(store[i].ident)) != server.end())
        {
            if (store[i].flags == EV_ERROR)
                serverError(store[i]);  //연결된 모든 client 종료
            else if (store[i].filter == EVFILT_READ)
                plusClient(static_cast<int>(store[i].ident));
        }
        else
        {
            if (store[i].flags == EV_ERROR)
                clientFin(store[i]);  //client 종료
            else if (store[i].filter == EVFILT_READ)
            {
                // else if (store[i].flags == EOF)
                //     std::cout<<"EOF"<<std::endl;
                eventRead(store[i]);
            }
            else if (store[i].filter == EVFILT_WRITE)
                eventWrite(store[i]);
        }
    }
}

void    Kq::clientFin(struct kevent& store)
{
    int     serverFd;

    // std::cout<<"error"<<std::endl;
    serverFd = findServer[store.ident];
    server[serverFd].clientFin(store.ident);
}

void    Kq::serverError(struct kevent& store)
{
    //client 모두 닫기
    //server 닫기
    Server  temp = server[store.ident];

    //server file discriptor가 에러가 나왔을 때에 연결된 클라이언트의 모든 것을 에러 처리한다.
    temp.serverError();
}
