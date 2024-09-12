/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 10:56:55 by inghwang          #+#    #+#             */
/*   Updated: 2024/09/01 21:53:48 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/event.h>
# include <sys/socket.h>
# include <unistd.h>
# include <fcntl.h>
# include <algorithm>
# include <cstdlib>
# include <string>
# include <arpa/inet.h>
# include "Client.hpp"
# include "HTTPServer.hpp"
# include "Kq.hpp"

# define BUFFER_SIZE 65536
# define PIPE_BUFFER_SIZE 65536
# define DEFAULT_400_ERROR_PAGE "./resource/html/error/40x.html"
# define DEFAULT_500_ERROR_PAGE "./resource/html/error/50x.html"
# define CGI_ERROR_PAGE "/cgi-bin/error_page/errorPage.py"

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

class Client;

/***
 * @brief Server manage clients
 * @param serverFd server fd
 * @param port server port
 * @param client server manage clients map (client fd -> client class)
*/
class Server
{
    private:
        int                     serverFd;
        int                     port;
		std::map<int, string>   cgiContent;
		std::map<int, size_t>   cgiContentLength;
        std::map<int, Client>   client;  //client을 선언할때에 default 생성자가 필요한듯
        //여기에 파싱된 data가 들어가 있을 것 - 아래 서버 스태틱으로 설정되어 있음
    public:
        static HTTPServer   *serverConfig;
        Server();
        Server(int fd, int num);  //파싱된 데이터도 인자로 추가할 것
        Server(const Server& src);
        Server &operator=(const Server& src);
        ~Server();
        //get function
        int                     getServerFd(void) const;
        int                     getPort(void) const;
        std::map<int, size_t>   getCgiContentLength(void) const;
        std::map<int, Client>   getClient(void) const;
        std::map<int, Client>   &getClient(void);
        //logic
        bool    getResponseCgi(int fd);
        ssize_t getStandardTime(int fd);
        int     plusClient(string pathEnv);	//client socket accept
        EVENT   cgiGet(struct kevent& store);
		EVENT	cgiRead(struct kevent& store);		//client cgi read event manage
        EVENT   clientRead(struct kevent& store);   //client read event manage
        EVENT   clientWrite(struct kevent& store);  //client write event manage
        EVENT   clientTimer(struct kevent& store);  //client timer event manage
        //error
        void    clientFin(int clientFd);    //one client close
        void    serverError();              //clients connected server close
};

#endif
