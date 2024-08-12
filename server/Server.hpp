/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 10:56:55 by inghwang          #+#    #+#             */
/*   Updated: 2024/08/12 13:59:12 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <sys/event.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm>

//temp
#include <cstdlib>
#include <string>
#include <arpa/inet.h>
#include "Client.hpp"

# define BUFFER_SIZE 4096

//server를 여러 개 만들 경우에는 kq클래스 만들기
//kq 클래스에서 서버 클래스를 관리하기
//kq 클래스에서 클라이언트 fd마다 서버 fd를 map을 통해 갖고 있기 때문에 event 발생시 클라이언트 fd를 통해 server를 예측하고 server에서 다시 클라이언트 fd를 통해 클라이언트 소켓으로 접근

class HTTPServer;

enum EVENT
{
    ERROR = 1,
    FINISH,
    ING,
    EXPECT
};

class Server
{
    private:
        int                     serverFd;
        std::map<int, Client>   client;  //client을 선언할때에 default 생성자가 필요한듯
        //여기에 파싱된 data가 들어가 있을 것
    public:
        Server();
        Server(int fd);  //파싱된 데이터도 인자로 추가할 것
        Server(const Server& src);
        Server &operator=(const Server& src);
        ~Server();
        static HTTPServer *serverConfig;
        //get function
        int getServerFd(void) const;
        std::map<int, Client>  getClient(void) const;
        //logic
        int     plusClient(void);
        EVENT   clientRead(struct kevent& store);
        EVENT   clientWrite(struct kevent& store);
        //error
        void    clientFin(int clientFd);
        void    serverError();
};

#endif
