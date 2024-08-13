/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 10:56:52 by inghwang          #+#    #+#             */
/*   Updated: 2024/08/12 21:30:17 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

extern int logs;

HTTPServer *Server::serverConfig = NULL;

Server::Server()
{}

Server::Server(int fd, int port) : serverFd(fd), port(port)
{}

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
    int                 clntFd;
    struct sockaddr_in  clntAdr;
    socklen_t           adrSize;

    adrSize = sizeof(clntAdr);
    //accept 무한 루프
    while ((clntFd = accept(serverFd, (struct sockaddr *)&clntAdr, &adrSize)) < 0);
    std::cout<<port<<std::endl;
    client[clntFd] = Client(clntFd, port);
    std::cout<<"server port: "<<port<<std::endl;
    std::cout<<"temp delete"<<std::endl;
    return (clntFd);
    // plusEvent(clntFd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
    //나갈 때 소멸자가 호출됨
}

EVENT Server::clientRead(struct kevent& store)
{
    //buffer 문제인지 생각해보기
    char    buffer[BUFFER_SIZE];
    int     readSize;

    //eof신호를 못 받게 됨
    if (client[store.ident].getRequestFin() || client[store.ident].getRequestStatus() > 100)
        return (ING);
    readSize = read(store.ident, buffer, BUFFER_SIZE);
    if (readSize <= 0) // read가 발생했는데 읽은게 없다면 에러
    {
        std::cout<<"read error or socket close\n";
        return (ERROR);
    }
    buffer[readSize] = '\0';
    client[store.ident].setMessage(buffer);
    if (client[store.ident].getRequestFin() || client[store.ident].getRequestStatus() > 0)
    {
        // request 완성 -> respond 만들면 되지 않나?
        client[store.ident].setResponseMessage();
        if (client[store.ident].getRequestStatus() == 100)
            return (EXPECT);
        if (client[store.ident].getRequestFin())
            client[store.ident].showMessage();
        return (FINISH);
    }
    return (ING);
}

EVENT   Server::clientWrite(struct kevent& store)
{
    size_t      index;
    const char* buffer = client[store.ident].respondMsgIndex();

    if (store.ident == 0)
        return (ING);
    (void) port;  //make 옵션 때문에
    index = write(store.ident, buffer, client[store.ident].responseIndex());
    // write(1, buffer, client[store.ident].getAmount());
    client[store.ident].plusIndex(index);
    if (client[store.ident].responseIndex())
        return (ING);
    if (client[store.ident].getRequestStatus() == 100)
        return (EXPECT);
    return (FINISH);
}

void    Server::clientFin(int clientFd)
{
    close(clientFd);
    client.erase(clientFd);
}

void    Server::serverError()
{
    //이벤트 해지는 하지 않고 socket만 닫기
    std::map<int, Client>::iterator it;

    for (it = client.begin(); it != client.end(); it++)
    {
        if (it->first == 0)
            continue ;
        clientFin(it->first);
    }
    close(serverFd);
}
