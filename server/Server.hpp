/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inghwang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 10:56:55 by inghwang          #+#    #+#             */
/*   Updated: 2024/07/08 10:56:56 by inghwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "../client/Client.hpp"
#include <sys/event.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm>

//temp
#include <cstdlib>
#include <string>
#include <arpa/inet.h>

# define BUFFER_SIZE 4096
# define PORT 8000
# define CLIENT_CNT 10
# define EVENTCNT 10

//server를 여러 개 만들 경우에는 kq클래스 만들기
//kq 클래스에서 서버 클래스를 관리하기
//kq 클래스에서 클라이언트 fd마다 서버 fd를 map을 통해 갖고 있기 때문에 event 발생시 클라이언트 fd를 통해 server를 예측하고 server에서 다시 클라이언트 fd를 통해 클라이언트 소켓으로 접근

enum EVENT
{
    ERROR = 1,
    FINISH,
    ING
};

class Server
{
    private:
        int                     serverFd;
        char                    buffer[BUFFER_SIZE];
        std::map<int, Client>   client;  //client을 선언할때에 default 생성자가 필요한듯
    public:
        static std::map<int, std::string>   statusContent;
        Server();
        Server(int fd);
        Server(const Server& src);
        Server &operator=(const Server& src);
        ~Server();
        //get function
        int getServerFd(void) const;
        std::map<int, Client>  getClient(void) const;
        //logic
        int     plusClient(void);
        EVENT   clientRead(int clientFd);
        EVENT   clientWrite(int clientFd);
        //error
        void    errorHandler(Client& c);
        void    clientError(int clientFd);
};

#endif
