/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kq.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inghwang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:08:58 by inghwang          #+#    #+#             */
/*   Updated: 2024/07/26 11:09:00 by inghwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Kq.hpp"

extern int logs;

Kq::Kq()
{
    struct sockaddr_in  serverAdr;
    int                 option;
    int                 serverFd;

    while ((kq = kqueue()) < 0);
    option = 1;
    //여기서 루프 돌면서 server socket전부다 만들기
    while ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) <= 0);
    while (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0);
    memset(&serverAdr, 0, sizeof(serverAdr));
    serverAdr.sin_family = AF_INET;
    serverAdr.sin_addr.s_addr = htonl(INADDR_ANY);  //ip를 어떻게 가져오는 방향에 대해 고민하기
    serverAdr.sin_port = htons(PORT);   //port도 마찬가지로 어떻게 가져오는지
    while (bind(serverFd, (struct sockaddr *)&serverAdr, sizeof(serverAdr)) < 0);
    while (listen(serverFd, CLIENT_CNT) < 0);
    plusEvent(serverFd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
    server[serverFd] = Server(serverFd);
}

Kq::Kq(const Kq& src)
{
    kq = src.getKq();
    fdList = src.getFdList();
    server = src.getServer();
}

Kq& Kq::operator=(const Kq& src)
{
    kq = src.getKq();
    fdList = src.getFdList();
    server = src.getServer();
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
    plusEvent(clientFd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
    findServer[clientFd] = serverFd;
}

void    Kq::eventRead(struct kevent& store)
{
    int     serverFd;
    EVENT   event;

    serverFd = findServer[store.ident];
    event = server[serverFd].clientRead(store.ident);
    switch (event)
    {
        case ERROR:
            //연결 종료하기
            break ;
        case ING:
            break ;
        case FINISH:
            EV_SET(&store, store.ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
            plusEvent(store.ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
            break ;
    }
}

void    Kq::eventWrite(struct kevent& store)
{
    int     serverFd;
    EVENT   event;

    serverFd = findServer[store.ident];
    event = server[serverFd].clientWrite(store.ident);
    switch (event)
    {
        case ERROR:
            //연결 종료하기
            break ;
        case ING:
            break ;
        case FINISH:
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
                clientError(store[i]);  //client 종료
            else if (store[i].filter == EVFILT_READ)
                eventRead(store[i]);
            else if (store[i].filter == EVFILT_WRITE)
                eventWrite(store[i]);
        }
    }
}

void    Kq::clientError(struct kevent& store)
{
    int     serverFd;

    serverFd = findServer[store.ident];
    server[serverFd].clientError(store.ident);
    EV_SET(&store, store.ident, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
    EV_SET(&store, store.ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
}

void    Kq::serverError(struct kevent& store)
{
    //client 모두 닫기
    //server 닫기
    EV_SET(&store, store.ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
}





