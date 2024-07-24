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
# define PORT 8080
# define CLIENT_CNT 10
# define EVENTCNT 10

//server를 여러 개 만들 경우에는 kq클래스 만들기
//kq 클래스에서 서버 클래스를 관리하기
//kq 클래스에서 클라이언트 fd마다 서버 fd를 map을 통해 갖고 있기 때문에 event 발생시 클라이언트 fd를 통해 server를 예측하고 server에서 다시 클라이언트 fd를 통해 클라이언트 소켓으로 접근

class Server
{
    private:
        std::vector<struct kevent>  fdList;
        std::map<std::string, std::string>  respond;
        int serverFd;
        int kq;
        std::map<int, Client>  client;
    public:
        Server();
        Server(const Server& src);
        Server &operator=(const Server& src);
        ~Server();
        //get function
        std::vector<struct kevent>  getFdList(void) const;
        std::map<std::string, std::string>  getRespond(void) const;
        int getServerFd(void) const;
        int getKq(void) const;
        std::map<int, Client>  getClient(void) const;
        //Server assistant function
        void    errorHandler(std::string message);
        void    nullSet(void *ptr, size_t len);
        //Server main function
        void    plusEvent(uintptr_t fd, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata);
        void    plusClient(void);
        void    mainLoop(void);
};

#endif
