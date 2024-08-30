/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 10:56:52 by inghwang          #+#    #+#             */
/*   Updated: 2024/08/30 14:40:53 by minsepar         ###   ########.fr       */
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
	readSize = read(store.ident, buf, BUFFER_SIZE);
	LOG(cout << "CGI Read Size : " << readSize << endl);
	if (readSize <= 0)
	{
        size_t  status = 0;
        LOG(std::cout<<"ERROR Kq::cgiFd[store.ident] : "<<Kq::cgiFd[store.ident]<<std::endl);
        std::cout<<"ERROR Kq::cgiFd[store.ident] : "<<Kq::cgiFd[store.ident]<<std::endl;
        client[Kq::cgiFd[store.ident]].setCgiResponseEntity(cgiContentLength[store.ident], cgiContent[store.ident], status);
        // LOG(cout<<"status: "<<status<<endl);
        cgiContent[store.ident].clear();  //이부분은 말이 안됨 동시에 여러개를 처리할 가능성이 있음
        // cgiContentLength[store.ident] = 0;
        cgiContentLength.erase(store.ident);
        if (status >= 400)
            return (ERROR);
        LOG(cout << Kq::cgiFd[store.ident] << endl);
        return (FINISH);
	}
    // close(1);
    buf[readSize] = '\0';
    cgiContent[store.ident].append(buf, readSize);  //인자값으로 const char이 가능함
    LOG(std::cout<<"cgiContent: "<<cgiContent[store.ident]<<std::endl);

    // LOG(std::cout<<cgiContent<<std::endl);
    // LOG(std::cout<<cgiContentLength<<std::endl);
    cgiContentLength[store.ident] += readSize;
    // LOG(cout << "hi: " <<cgiContentLength << endl);
	// if (readSize < PIPE_BUFFER_SIZE)
    // {
    //     LOG(std::cout<<"FINISH Kq::cgiFd[store.ident]: "<<Kq::cgiFd[store.ident]<<std::endl);
    //     //pipe fd를 갖는 새로운 client이므로 새로운 request.status를 갖는다. 따라서 쓰레기 값이 들어감(정답)
        // 요거 지워야 함
		// client[Kq::cgiFd[store.ident]].setResponseContentLength(cgiContentLength);
    //     client[Kq::cgiFd[store.ident]].setResponseContent(cgiContentLength, cgiContent);
	// 	cgiContent.clear();
	// 	cgiContentLength = 0;
    //     // LOG(std::cout<<"msg\n"<<client[Kq::cgiFd[store.ident]].getMsg());
    //     // LOG(std::cout<<"====================="<<std::endl);
	// 	return (FINISH);
    // }
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
    LOG(std::cout<<store.ident<<" "<<client[store.ident].responseIndex()<<std::endl);
    write(writeLogs, buffer, client[store.ident].responseIndex());
    // write(1, buffer, client[store.ident].responseIndex());
    index = write(store.ident, buffer, client[store.ident].responseIndex());
    client[store.ident].plusIndex(index);
    client[store.ident].setConnection(true);
    if (client[store.ident].responseIndex())
        return (ING);
    if (client[store.ident].getRequestStatus() == 100)
        return (EXPECT);
    client[store.ident].deleteContent();
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
