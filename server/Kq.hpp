/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kq.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:09:01 by inghwang          #+#    #+#             */
/*   Updated: 2024/08/12 13:31:17 by minsepar         ###   ########.fr       */
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

class Kq
{
    private:
        int                         kq;  //kq 저장 공간
        std::vector<struct kevent>  fdList;  //kq 이벤트 등록 저장공간
        std::map<int, Server>       server;  //server fd -> server class
        std::map<int, int>          findServer;  //client fd -> server fd
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
        //logic
        void    plusEvent(uintptr_t fd, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata); /* abc */
        void    plusClient(int serverFd);
        void    eventRead(struct kevent& store);
        void    eventWrite(struct kevent& store);
        void    mainLoop();
        //error
        void    clientFin(struct kevent& store);
        void    serverError(struct kevent& store);
};

#endif
