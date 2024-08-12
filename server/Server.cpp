/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 10:56:52 by inghwang          #+#    #+#             */
/*   Updated: 2024/08/12 13:59:17 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

extern int logs;

HTTPServer *Server::serverConfig = NULL;

Server::Server()
{}

Server::Server(int fd)
{
    //여기서 startline method설정해야 함
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
    int                 clntFd;
    struct sockaddr_in  clntAdr;
    socklen_t           adrSize;

    adrSize = sizeof(clntAdr);
    //accept 무한 루프
    while ((clntFd = accept(serverFd, (struct sockaddr *)&clntAdr, &adrSize)) < 0);
    client[clntFd] = Client(clntFd);
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
    if (readSize <= 0)
    {
        std::cout<<"read error or socket close\n";
        return (ERROR);
    }
    buffer[readSize] = '\0';
    client[store.ident].setMessage(buffer);
    if (client[store.ident].getRequestFin() || client[store.ident].getRequestStatus() > 0)
    {
        // request 완성 -> respond 만들면 되지 않나?
        client[store.ident].getResponseMessage();
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
    const char* buffer = client[store.ident].getMsg();

    if (store.ident == 0)
        return (ING);
    index = write(store.ident, buffer, client[store.ident].getAmount());
    write(1, buffer, client[store.ident].getAmount());
    index = write(store.ident, buffer, 100);
    // write(1, buffer, 100);
    // std::cout<<buffer;
    client[store.ident].plusIndex(index);
    if (client[store.ident].getAmount())
        return (ING);
    if (client[store.ident].getRequestStatus() == 100)
        return (EXPECT);
    return (FINISH);
}

// EVENT Server::clientWrite(struct kevent& store)
// {
//     // write(store.ident, client[store.ident].getMsg().c_str(), client[store.ident].getMsg().size());
//     int     sum;
//     int     readSize;
//     int     fd;
//     char    buffer[BUFFER_SIZE];
//     std::string str;
//     const char  *temp = "HTTP/1.1 200 OK\nContent-Type: text/html;charset=UTF-8\nContent-Length: ";

//     if (client[store.ident].getRequestStatus() == 100 && !client[store.ident].getRequestFin())
//     {
//         write(store.ident, "HTTP/1.1 100 Continue\n", 22);
//         client[store.ident].setRequestStatus(0);
//         return (EXPECT);
//     }
//     if (client[store.ident].getRequestStatus() > 0)
//         errorHandler(client[store.ident]);
//     else
//     {
//         fd = open("./resource/index.html", O_RDONLY);
//         sum = 0;
//         while (1)
//         {
//             readSize = read(fd, buffer, BUFFER_SIZE);
//             if (readSize <= 0)
//                 break;
//             sum += readSize;
//         }
//         close(fd);
//         write(store.ident, temp, strlen(temp));
//         str = std::to_string(sum);
//         write(store.ident, str.c_str(), str.size());
//         write(store.ident, "\n\n", 2);
//         fd = open("./resource/index.html", O_RDONLY);
//         while (1)
//         {
//             readSize = read(fd, buffer, BUFFER_SIZE);
//             if (readSize <= 0)
//                 break;
//             write(store.ident, buffer, strlen(buffer));
//         }
//         close(fd);
//     }
//     // close(static_cast<int>(clientFd));
//     // client.erase(clientFd);
//     return (FINISH);
// }

// void    Server::errorHandler(Client& c)
// {
//     std::string temp;
//     int         readSize;
//     char        buffer[BUFFER_SIZE];
//     int         fd;
//     int         sum;

//     //시작줄
//     write(c.getFd(), "HTTP/1.1 ", 9);
//     temp = std::to_string(c.getRequestStatus());
//     temp += " " + statusContent[c.getRequestStatus()];
//     write(c.getFd(), temp.c_str(), temp.size());
//     //헤더줄
//     write(c.getFd(), "\nContent-Type: text/html;charset=UTF-8\nContent-Length: ", 55);
//     sum = 0;
//     fd = open("./resource/html/error.html", O_RDONLY);
//     while (1)
//     {
//         readSize = read(fd, buffer, BUFFER_SIZE);
//         if (readSize <= 0)
//             break;
//         sum += readSize;
//     }
//     close(fd);
//     temp = std::to_string(sum);
//     write(c.getFd(), temp.c_str(), temp.size());
//     write(c.getFd(), "\n\n", 2);
//     //본문
//     fd = open("./resource/html/error.html", O_RDONLY);
//     while (1)
//     {
//         readSize = read(fd, buffer, BUFFER_SIZE);
//         if (readSize <= 0)
//             break;
//         write(c.getFd(), buffer, strlen(buffer));
//     }
//     close(fd);
// }

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
