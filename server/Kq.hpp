/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kq.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:09:01 by inghwang          #+#    #+#             */
/*   Updated: 2024/08/12 22:28:00 by minsepar         ###   ########.fr       */
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

/**
 * @brief kqueue를 사용하여 이벤트를 관리하는 클래스
 * @param kq kqueue fd
 * @param fdList 이벤트가 발생한 fd를 저장하는 리스트
 * @param server server fd를 통해 server를 찾기 위한 map
 * @param findServer client fd를 통해 server fd를 찾기 위한 map
*/
class Kq
{
    private:
        int                         kq;
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
        //logic
        void    plusEvent(uintptr_t fd, int16_t filter, uint16_t flags,
                    uint32_t fflags, intptr_t data, void *udata); //이벤트를 추가하는 함수
        /**
         * @brief client를 추가하는 함수
        */
        void    plusClient(int serverFd);
        void    eventRead(struct kevent& store); 
        void    eventWrite(struct kevent& store);
        void    mainLoop();
        //error
        void    clientFin(struct kevent& store);
        void    serverError(struct kevent& store);
};

#endif
