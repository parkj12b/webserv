/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 10:56:52 by inghwang          #+#    #+#             */
/*   Updated: 2024/08/19 14:49:04 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <algorithm>

extern int logs;

HTTPServer *Server::serverConfig = NULL;

Server::Server()
{}

Server::Server(int fd, int num) : serverFd(fd), port(num)
{}

Server::Server(const Server& src) : serverFd(src.getServerFd()), port(src.getPort()), client(src.getClient())
{}

Server&  Server::operator=(const Server& src)
{
    serverFd = src.getServerFd();
    port = src.getPort();
    client = src.getClient();
    return (*this);
}

Server::~Server()
{}

int Server::getServerFd(void) const
{
    return (serverFd);
}

int Server::getPort(void) const
{
    return (port);
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
    client[clntFd] = Client(clntFd, port);
	client[clntFd].clientIP(clntAdr);
    std::cout<<"temp delete"<<std::endl;
    return (clntFd);
    // 나갈 때 소멸자가 호출됨
}

EVENT Server::clientRead(struct kevent& store)
{
    //buffer 문제인지 생각해보기
    std::vector<Client*>::iterator   it;
    char    buffer[BUFFER_SIZE + 1];
    int     readSize;

    //eof신호를 못 받게 됨
    if (store.ident == 0 || client[store.ident].getFd() == 0)
        return (ING);
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
    client[store.ident].setConnection(true);
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
    std::vector<Client*>::iterator   it;
    size_t      index;
    const char* buffer = client[store.ident].respondMsgIndex();

    if (store.ident == 0 || client[store.ident].getFd() == 0)
        return (ING);
    index = write(store.ident, buffer, client[store.ident].responseIndex());
    client[store.ident].plusIndex(index);
    client[store.ident].setConnection(true);
    if (client[store.ident].responseIndex())
        return (ING);
    if (client[store.ident].getRequestStatus() == 100)
        return (EXPECT);
    if (!client[store.ident].getConnect())
    {
        std::cout<<"connection fin"<<std::endl;
        return (ERROR);
    }
    client[store.ident].resetClient();
    std::cout<<"keep-alive"<<std::endl;
    return (FINISH);
}

// EVENT   Server::clientTimer(struct kevent& store)
// {
//     bool    flag;

//     if (store.ident == 0 || client[store.ident].getFd() == 0)
//         return (ING);
//     flag = client[store.ident].getConnection();
//     client[store.ident].setConnection(false);
//     if (flag)
//         return (ING);
//     return (FINISH);
// }

void    Server::clientFin(int clientFd)
{
    std::vector<Client*>::iterator it;

    close(clientFd);
    // it = std::find(Kq::clientKeepAlive.begin(), Kq::clientKeepAlive.end(), &client[clientFd]);
    // if (it != Kq::clientKeepAlive.end())
    // {
    //     std::cout<<"NULL"<<std::endl;
    //     *it = NULL;
    // }
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
