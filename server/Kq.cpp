/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kq.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:08:58 by inghwang          #+#    #+#             */
/*   Updated: 2024/08/15 02:07:26 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Kq.hpp"

extern int logs;

std::vector<pid_t>  processorInit()
{
    std::vector<pid_t>  v;

    return (v);
}

std::vector<pid_t>      Kq::processor = processorInit();

Kq::Kq()
{
    struct sockaddr_in  serverAdr;
    int                 option;
    int                 serverFd;
    //Changing portNum to config port
    map<int, map<string, ServerConfigData *> > &serverConfigData = Server::serverConfig->getServerConfigData();
    map<int, map<string, ServerConfigData *> >::iterator serverConfigIt = serverConfigData.begin();

    while ((kq = kqueue()) < 0);
    option = 1;
    //여기서 루프 돌면서 server socket전부다 만들기
    while (serverConfigIt != serverConfigData.end())  //config parser
    {
        int port = serverConfigIt->first;
        while ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) <= 0);
        while (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0);
        memset(&serverAdr, 0, sizeof(serverAdr));
        serverAdr.sin_family = AF_INET;
        serverAdr.sin_addr.s_addr = htonl(INADDR_ANY);  //ip는 무조건 localhost로. config에서 에러 처리
        serverAdr.sin_port = htons(port);   //config parser
        while (::bind(serverFd, (struct sockaddr *)&serverAdr, sizeof(serverAdr)) < 0)
        {
            if (errno == EADDRINUSE)  //ip 에러를 여기서 처리할 수도...
                std::exit(1);
        }
        while (listen(serverFd, CLIENT_CNT) < 0);
        plusEvent(serverFd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
        server[serverFd] = Server(serverFd, port);  //config parser
        serverConfigIt++;
    }
    std::cout<<"server port open.\n";
    connectionCnt = Server::serverConfig->getWorkerConnections();
}

Kq::Kq(const Kq& src) : kq(src.getKq()), fdList(src.getFdList()), server(src.getServer()), findServer(src.getFindServer())
{}

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

void    Kq::clientFin(struct kevent& store)
{
    int     serverFd;

    std::cout<<"bye"<<std::endl;
    serverFd = findServer[store.ident];
    plusEvent(store.ident, EVFILT_TIMER, EV_DELETE, 0, 0, 0);
    // plusEvent(store.ident, EVFILT_WRITE, EV_DELETE, 0, 0, 0);
    // plusEvent(store.ident, EVFILT_READ, EV_DELETE, 0, 0, 0);
    findServer[store.ident] = 0;
    server[serverFd].clientFin(store.ident);
}

void    Kq::serverError(struct kevent& store)
{
    //client 모두 닫기
    //server 닫기
    Server  temp = server[store.ident];

    temp.serverError();
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
    if (clientFd < 0)
        return ;
    std::cout<<"plus client "<<clientFd<<std::endl;
    plusEvent(clientFd, EVFILT_TIMER, EV_ADD | EV_ENABLE, 0, 50000, 0);  //50초
    plusEvent(clientFd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
    findServer[clientFd] = serverFd;
}

void    Kq::eventRead(struct kevent& store)
{
    int     serverFd;
    EVENT   event;

    if (store.ident == 0)
        return ;
    serverFd = findServer[store.ident]; // client fd (store.ident) 이벤트 발생 fd 를 통해 server fd를 찾음
    if (serverFd == 0)
        return ;
    event = server[serverFd].clientRead(store);
    switch (event)
    {
        case ERROR:
            clientFin(store);
            break ;
        case ING:
            break ;
        case EXPECT:
        case FINISH:
            plusEvent(store.ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
            plusEvent(store.ident, EVFILT_TIMER, EV_DELETE, 0, 0, 0);
            plusEvent(store.ident, EVFILT_TIMER, EV_ADD | EV_ENABLE, 0, 75000, 0);  //75초
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
        case ING:
            break ;
        case ERROR:
            clientFin(store);
            break ;
        case FINISH:
        case EXPECT:
            plusEvent(store.ident, EVFILT_WRITE, EV_DELETE, 0, 0, 0);
            break ;
    }
}

void    Kq::eventTimer(struct kevent& store)
{
    int     serverFd;
    EVENT   event;

    serverFd = findServer[store.ident];
    if (serverFd == 0)
        return ;
    event = server[serverFd].clientTimer(store);
    switch (event)
    {
        case EXPECT:
        case ERROR:
        case ING:
            break ;
        case FINISH:
            clientFin(store);
            break ;
    }
}

void    Kq::mainLoop()
{
    std::vector<pid_t>  notFin;
    struct kevent       store[connectionCnt];
    int                 count;

    //waitpid
    for (std::vector<pid_t>::iterator it = Kq::processor.begin(); it != Kq::processor.end(); it++)
    {
        if (waitpid(*it, NULL, WNOHANG) == 0)
            notFin.push_back(*it);
    }
    Kq::processor = notFin;
    //changed EVENTCNT to connectionCnt
    if ((count = kevent(kq, &fdList[0], fdList.size(), store, connectionCnt, NULL)) <= 0)
        return ;
    fdList.clear();
    for (int i = 0; i < count; i++)
    {
        if (server.find(static_cast<int>(store[i].ident)) != server.end())
        {
            if (store[i].flags == EV_ERROR)
                serverError(store[i]);  //server에 연결된 모든 client 종료
            else if (store[i].filter == EVFILT_READ) //read 발생시 client 추가
                plusClient(static_cast<int>(store[i].ident));
        }
        else
        {
            std::cout<<"store[i].ident: "<<store[i].ident<<std::endl;
            if (store[i].flags == EV_ERROR)
                clientFin(store[i]);  //client 종료
            else if (store[i].filter == EVFILT_READ)
                eventRead(store[i]);
            else if (store[i].filter == EVFILT_WRITE)
                eventWrite(store[i]);
            else if (store[i].filter == EVFILT_TIMER)
                eventTimer(store[i]);
        }
    }
}

