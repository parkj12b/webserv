/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kq.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:09:01 by inghwang          #+#    #+#             */
/*   Updated: 2024/08/15 01:10:31 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KQ_HPP
# define KQ_HPP

# include "Server.hpp"
# include <cstdlib>
# include <cerrno>
# include <map>
# include <vector>

class HTTPServer;

# define PORT 8000
# define CLIENT_CNT 10
# define EVENTCNT 10

/***
 * @brief Kq manages server and kq
 * @param kq kq save space
 * @param fdList kq 이벤트 등록 저장공간
 * @param server server fd -> server class
 * @param findServer client fd -> server fd
*/
class Kq
{
    private:
        int                         kq;
        int                         connectionCnt;
        std::vector<struct kevent>  fdList;
        std::map<int, Server>       server;
        std::map<int, int>          findServer;
    public:
        Kq();
        Kq(const Kq& src);
        Kq& operator=(const Kq& src);
        ~Kq();
        //get function
        int                         getKq() const;
        std::vector<struct kevent>  getFdList() const;
        std::map<int, Server>       getServer() const;
        std::map<int, int>          getFindServer() const;
        //error
        void    clientFin(struct kevent& store);    //client error client socket close
        void    serverError(struct kevent& store);  //server error clients connected server and server sockets close
        //logic
        void    plusEvent(uintptr_t fd, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata);   //event enoll
        void    plusClient(int serverFd);           //client and server connect
        void    eventRead(struct kevent& store);    //kq event read
        void    eventWrite(struct kevent& store);   //kq event write
        void    mainLoop();                         //main logic
};

#endif
