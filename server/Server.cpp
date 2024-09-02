/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 10:56:52 by inghwang          #+#    #+#             */
/*   Updated: 2024/09/01 21:41:10 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include "Server.hpp"
#include "Response.hpp"
#include "UtilTemplate.hpp"


extern int logs;
extern int writeLogs;

HTTPServer *Server::serverConfig = NULL;

Server::Server() : serverFd(0), port(0)
{}

Server::Server(int fd, int num) : serverFd(fd), port(num)
{}

Server::Server(const Server& src) : serverFd(src.getServerFd()), port(src.getPort()), cgiContentLength(src.cgiContentLength), client(src.getClient())
{}

Server&  Server::operator=(const Server& src)
{
    serverFd = src.getServerFd();
    port = src.getPort();
    client = src.getClient();
    cgiContentLength = src.getCgiContentLength();
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

std::map<int, size_t>   Server::getCgiContentLength(void) const
{
    return (cgiContentLength);
}

std::map<int, Client>  Server::getClient(void) const
{
    return (client);
}

bool    Server::getResponseCgi(int fd)
{
    return (client[fd].getResponseCgi());
}

ssize_t Server::getStandardTime(int fd)
{
    ssize_t   standardTime = client[fd].getStandardTime();

    if (standardTime < 0)
        return (75000);
    return (standardTime * 1000);
}

void setLinger(int sockfd, int linger_time) {
    struct linger linger_opt;
    linger_opt.l_onoff = 1;  // Enable linger option
    linger_opt.l_linger = linger_time;  // Set linger time in seconds

    if (setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &linger_opt, sizeof(linger_opt)) < 0) {
        perror("setsockopt(SO_LINGER) failed");
    }
    //fcntl temp
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

int Server::plusClient(string pathEnv)
{
    int                 clntFd;
    struct sockaddr_in  clntAdr;
    socklen_t           adrSize;

    adrSize = sizeof(clntAdr);
    //accept 무한 루프 && server 동기적 실패시 무한 루프 가능성 
    if ((clntFd = accept(serverFd, (struct sockaddr *)&clntAdr, &adrSize)) < 0)
        return (-1);
    setLinger(clntFd, 0);
    fcntl(clntFd, F_SETFL, O_NONBLOCK);
    client[clntFd] = Client(clntFd, port, pathEnv);  //생성자 및 대입 연산자 호출
	client[clntFd].clientIP(clntAdr);
    LOG(std::cout<<"temp delete"<<std::endl);
    return (clntFd);
}

EVENT Server::cgiRead(struct kevent& store)
{
	char	    buf[BUFFER_SIZE + 1];  //BUFFER_SIZE의 크기를 65536로 조절하였습니다. 
	int         readSize;

	LOG(cout << "cgiRead fd: " << store.ident << endl);
    //초기 세팅
    if (cgiContentLength.find(store.ident) == cgiContentLength.end())
    {
        cgiContentLength[store.ident] = 0;
        cgiContent[store.ident].clear();
    }
    size_t  status = 0;
    int     waitStatus = 0;
    waitpid(Kq::pidPipe[store.ident], &waitStatus, WNOHANG);
    if (WEXITSTATUS(waitStatus) != 0)
    {
        write(2, "ERROR\n", 6);
        status = 600;
    }
    if (WIFEXITED(waitStatus))
    {
        status = WEXITSTATUS(waitStatus);
        LOG(std::cout<<"status: "<<status<<std::endl);
    }
	readSize = read(store.ident, buf, BUFFER_SIZE);
	LOG(cout << "CGI Read Size : " << readSize << endl);
	if (readSize <= 0)
	{
        LOG(std::cout<<"ERROR Kq::cgiFd[store.ident] : "<<Kq::cgiFd[store.ident]<<std::endl);
        client[Kq::cgiFd[store.ident]].setCgiResponseEntity(cgiContentLength[store.ident], cgiContent[store.ident], status);
        // LOG(cout<<"status: "<<status<<endl);
        cgiContent[store.ident].clear();
        cgiContentLength[store.ident] = 0;
        cgiContentLength.erase(store.ident);
        Kq::pidPipe.erase(store.ident);
        if (status >= 400)
            return (ERROR);
        LOG(cout << "status: " << status << endl);
        // LOG(std::cout << "msg: " << client[Kq::cgiFd[store.ident]].getMsg() << endl);
        return (FINISH);
	}
    buf[readSize] = '\0';
    cgiContent[store.ident].append(buf, readSize);
    cgiContentLength[store.ident] += readSize;
    // LOG(std::cout<<"cgi: "<<cgiContent[store.ident]<<std::endl;)
	return (ING);
}

EVENT Server::clientRead(struct kevent& store)
{
    //buffer 문제인지 생각해보기
    char    buffer[BUFFER_SIZE + 1];
    int     readSize;

    //eof신호를 못 받게 됨
    if (store.ident == 0 || client[store.ident].getFd() == 0)
        return (ING);
    // if (client[store.ident].getRequestFin() || client[store.ident].getRequestStatus() > 100)
    //     return (ING);
    readSize = read(store.ident, buffer, BUFFER_SIZE);
    if (readSize <= 0) // read가 발생했는데 읽은게 없다면 에러
    {
        LOG(std::cout<<"read error or socket close\n");
        client[store.ident].deleteContent();
        return (ERROR);
    }
    LOG(std::cout<<"Client Read " << readSize << std::endl);
    buffer[readSize] = '\0';
    write(logs, buffer, readSize);
    client[store.ident].setMessage(buffer, readSize);
    client[store.ident].setConnection(true);
    if (client[store.ident].getRequestFin() || client[store.ident].getRequestStatus() > 0)
    {
        client[store.ident].setResponseMessage();
        if (client[store.ident].getRequestStatus() == 100)
            return (EXPECT);
        //debug
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
    // int flags = fcntl(store.ident, F_GETFL, 0);
    // if (!(flags & O_RDWR))
    //     return (ERROR);
    cout<<"msg: " <<buffer<<endl;
    LOG(std::cout<<store.ident<<" "<<client[store.ident].responseIndex()<<std::endl);
    char    buf[1];
    ssize_t bytes_received = recv(store.ident, buf, sizeof(buf), MSG_PEEK);
    if (bytes_received == 0)
    {
        LOG(std::cout<<"write: client close"<<std::endl);
        return (ERROR);
    }
    // write(writeLogs, buffer, client[store.ident].responseIndex());
    // write(1, buffer, client[store.ident].responseIndex());
    index = write(store.ident, buffer, client[store.ident].responseIndex());
    if (index > client[store.ident].responseIndex())
        return (ERROR);
    std::cout<<index<<endl;
    // if (index > client[store.ident].responseIndex())
    // {
    //     std::cout<<"ERROR wirte"<<endl;
    //     return (ERROR);
    // }
    client[store.ident].plusIndex(index);
    client[store.ident].setConnection(true);
    if (client[store.ident].responseIndex())
        return (ING);
    if (client[store.ident].getRequestStatus() == 100)
        return (EXPECT);
    client[store.ident].deleteContent();
    client[store.ident].resetClient();
    if (!client[store.ident].getConnect())
    {
        LOG(std::cout<<"connection fin"<<std::endl);
        return (ERROR);
    }
    LOG(std::cout<<"keep-alive"<<std::endl);
    return (FINISH);
}

EVENT   Server::clientTimer(struct kevent& store)
{
    bool    flag;

    if (store.ident == 0 || client[store.ident].getFd() == 0)
        return (ING);
    flag = client[store.ident].getConnection();
    client[store.ident].setConnection(false);
    if (flag)
        return (ING);
    LOG(std::cout<<"TIMER OUT"<<std::endl);
    return (FINISH);
}

void    Server::clientFin(int clientFd)
{
    client[clientFd].deleteContent();
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
