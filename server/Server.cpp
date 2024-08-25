/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 10:56:52 by inghwang          #+#    #+#             */
/*   Updated: 2024/08/25 19:46:09 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include "Server.hpp"
#include "Response.hpp"
#include "UtilTemplate.hpp" 

extern int logs;

HTTPServer *Server::serverConfig = NULL;

Server::Server() : serverFd(0), port(0)
{}

Server::Server(int fd, int num) : serverFd(fd), port(num), cgiContentLength(0)
{}

Server::Server(const Server& src) : serverFd(src.getServerFd()), port(src.getPort()), cgiContentLength(src.getCgiContentLength()), client(src.getClient())
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

size_t  Server::getCgiContentLength(void) const
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
        return (7500);
    return (standardTime);
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
    client[clntFd] = Client(clntFd, port, pathEnv);
	client[clntFd].clientIP(clntAdr);
    LOG(std::cout<<"temp delete"<<std::endl);
    return (clntFd);
}

EVENT Server::cgiRead(struct kevent& store)
{
	char	buf[PIPE_BUFFER_SIZE + 1];
	int     readSize;

	LOG(cout << "cgiRead fd: " << store.ident << endl);
	readSize = read(store.ident, buf, PIPE_BUFFER_SIZE);
	LOG(cout << "CGI Read Size : " << readSize << endl);
	if (readSize <= 0)
	{
        LOG(std::cout<<"ERROR Kq::cgiFd[store.ident] : "<<Kq::cgiFd[store.ident]<<std::endl);
        // client[Kq::cgiFd[store.ident]].getResponse().setRequestStatus(500);
        client[Kq::cgiFd[store.ident]].setResponseContentLength(cgiContentLength);
        client[Kq::cgiFd[store.ident]].setResponseContent(cgiContentLength, cgiContent);
        LOG(cout << Kq::cgiFd[store.ident] << endl);
        // static error page
        // client[Kq::cgiFd[store.ident]].getResponse().makeError();
        client[Kq::cgiFd[store.ident]].setErrorMsg();
        cgiContent.clear();
        cgiContentLength = 0;
        return (ERROR);
	}
    // close(1);
    buf[readSize] = '\0';
    cgiContent.append(string(buf));
    // LOG(std::cout<<cgiContent<<std::endl);
    // LOG(std::cout<<cgiContentLength<<std::endl);
    cgiContentLength += readSize;
    // LOG(cout << "hi: " <<cgiContentLength << endl);
    // if (readSize < PIPE_BUFFER_SIZE)
	if (readSize < PIPE_BUFFER_SIZE)
    {
        LOG(std::cout<<"FINISH Kq::cgiFd[store.ident]: "<<Kq::cgiFd[store.ident]<<std::endl);
        //pipe fd를 갖는 새로운 client이므로 새로운 request.status를 갖는다. 따라서 쓰레기 값이 들어감(정답)
		client[Kq::cgiFd[store.ident]].setResponseContentLength(cgiContentLength);
        client[Kq::cgiFd[store.ident]].setResponseContent(cgiContentLength, cgiContent);
		cgiContent.clear();
		cgiContentLength = 0;
        // LOG(std::cout<<"msg\n"<<client[Kq::cgiFd[store.ident]].getMsg());
        // LOG(std::cout<<"====================="<<std::endl);
		return (FINISH);
    }
	return (ING);
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
        LOG(std::cout<<"read error or socket close\n");
        return (ERROR);
    }
    LOG(std::cout<<"Client Read" << " " << readSize << std::endl);
    buffer[readSize] = '\0';
    client[store.ident].setMessage(buffer, readSize);
    client[store.ident].setConnection(true);
    if (client[store.ident].getRequestFin() || client[store.ident].getRequestStatus() > 0)
    {
        LOG(std::cout<<"parsing complete"<<std::endl);
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
        LOG(std::cout<<"connection fin"<<std::endl);
        return (ERROR);
    }
    client[store.ident].resetClient();
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
