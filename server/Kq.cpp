/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kq.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: devpark <devpark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:08:58 by inghwang          #+#    #+#             */
/*   Updated: 2024/08/27 18:53:24 by devpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Kq.hpp"
#include "UtilTemplate.hpp" 

extern int logs;

std::vector<pid_t>  processorInit()
{
    std::vector<pid_t>  v;

    return (v);
}

std::vector<struct kevent>  fdListInit()
{
    std::vector<struct kevent>  m;

    return (m);
}

std::map<int, int>  cgiFdInit()
{
    std::map<int, int>  m;

    return (m);
}

std::vector<pid_t>  Kq::processor = processorInit();
std::vector<struct kevent> Kq::fdList = fdListInit();
std::map<int, int>  Kq::cgiFd = cgiFdInit();

Kq::Kq(string pathEnv_) : pathEnv(pathEnv_)
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
    LOG(std::cout<<"server port open.\n");
    connectionCnt = Server::serverConfig->getWorkerConnections();
}

Kq::Kq(const Kq& src) : kq(src.getKq()), connectionCnt(Server::serverConfig->getWorkerConnections()), pathEnv(src.pathEnv), server(src.getServer()), findServer(src.getFindServer())
{}

Kq& Kq::operator=(const Kq& src)
{
    kq = src.getKq();
    connectionCnt = Server::serverConfig->getWorkerConnections();
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

    LOG(std::cout<<"bye"<<std::endl);
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

    clientFd = server[serverFd].plusClient(pathEnv);
    if (clientFd < 0)
        return ;
    LOG(std::cout<<"plus client "<<clientFd<<std::endl);
    plusEvent(clientFd, EVFILT_TIMER, EV_ADD | EV_ENABLE, 0, 75000, 0);  //75초 default값
    plusEvent(clientFd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
    findServer[clientFd] = serverFd;
}

void    Kq::eventRead(struct kevent& store)
{
    int     serverFd;
    EVENT   event;
	map<int, int>::iterator iter = cgiFd.begin();

    if (store.ident == 0)
        return ;
	//vector에 담기 혹은 다른 방안 생각하기 serverFd가 map일 텐데 이거를 pipe일 경우에 value값을 1로
	while (iter != cgiFd.end())
	{
		if (iter->first == static_cast<int>(store.ident))
			break;
		iter++;
	}
	if (iter != cgiFd.end())
	{
        // LOG(std::cout<<"cgi here\n");
        LOG(std::cout<<"READ EVENT CGI"<<store.ident<<endl);
		serverFd = findServer[cgiFd[store.ident]]; // client fd (store.ident) 이벤트 발생 fd 를 통해 server fd를 찾음
		if (serverFd == 0)
        {
            // plusEvent(store.ident, EVFILT_READ, EV_DELETE, 0, 0, 0);
			return ;
        }
		event = server[serverFd].cgiRead(store);
		switch (event)
		{
			case EXPECT:
			case ING:
				break ;
			case ERROR:
			case FINISH:
                LOG(std::cout<<"[CGI Read FD, not Client FD] : "<<iter->first<<std::endl);
                plusEvent(store.ident, EVFILT_READ, EV_DELETE, 0, 0, 0);
                plusEvent(cgiFd[store.ident], EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
				close(iter->first);  //cgi에서 사용한 fd를 닫지 않음
				cgiFd[iter->first] = 0;
                cgiFd.erase(iter->first);
				break ;
		}
	}
	else
	{
        LOG(std::cout<<"READ EVENT CLIENT"<<store.ident<<endl);
		serverFd = findServer[store.ident]; // client fd (store.ident) 이벤트 발생 fd 를 통해 server fd를 찾음
		if (serverFd == 0)
        {
            LOG(std::cout<<"good"<<std::endl);
            // plusEvent(store.ident, EVFILT_READ, EV_DELETE, 0, 0, 0);
            // clientFin(store);
			return ;
        }
		event = server[serverFd].clientRead(store);
		switch (event)
		{
			case ERROR:
                plusEvent(store.ident, EVFILT_READ, EV_DELETE, 0, 0, 0);
				clientFin(store);
				break ;
			case ING:
				break ;
			case EXPECT:
			case FINISH:
                if (!server[serverFd].getResponseCgi(store.ident))  //cgi임을 체크하기 cgi임을 확인하고 write를 완료하면 response를 초기화를 진행한다. 그렇게 되면 여태까지 만들어놓은 response는 사라진다. 
                    plusEvent(store.ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
				plusEvent(store.ident, EVFILT_TIMER, EV_DELETE, 0, 0, 0);
				plusEvent(store.ident, EVFILT_TIMER, EV_ADD | EV_ENABLE, 0, server[serverFd].getStandardTime(store.ident), 0);  //75초
				break ;
		}
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
            plusEvent(store.ident, EVFILT_WRITE, EV_DELETE, 0, 0, 0);
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
            plusEvent(store.ident, EVFILT_TIMER, EV_DELETE, 0, 0, 0);
            clientFin(store);
            break ;
    }
}

void    Kq::mainLoop()
{
    // std::vector<pid_t>  notFin;
    struct kevent       store[connectionCnt];
    int                 count;

    //waitpid(complete) 복사 생성자를 없앰 다만 erase를 진행할 때의 오히려 비용이 조금 더 들 수도 있을 수도 있다. 
    for (std::vector<pid_t>::iterator it = Kq::processor.begin(); it != Kq::processor.end();)
    {
        if (waitpid(*it, NULL, WNOHANG) <= 0)
            it++;
        else
            it = Kq::processor.erase(it);
    }
    // Kq::processor = notFin;
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
            else if (store[i].filter == EVFILT_READ) //read event(complete)
                plusClient(static_cast<int>(store[i].ident));
        }
        else
        {
            // LOG(std::cout<<"store[i].ident: "<<store[i].ident<<std::endl);
            if (store[i].flags == EV_ERROR)
                clientFin(store[i]);  //client 종료
            else if (store[i].filter == EVFILT_READ)
            {
                // LOG(std::cout<<"read"<<" "<<store[i].ident<<std::endl);
                eventRead(store[i]);
            }
            else if (store[i].filter == EVFILT_WRITE)
            {
                LOG(std::cout<<"write"<<std::endl);
                eventWrite(store[i]);
            }
            else if (store[i].filter == EVFILT_TIMER)
            {
                LOG(std::cout<<"timer"<<std::endl);
                eventTimer(store[i]);
            }
        }
    }
}
