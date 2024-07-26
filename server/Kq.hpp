/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kq.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inghwang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:09:01 by inghwang          #+#    #+#             */
/*   Updated: 2024/07/26 11:09:02 by inghwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KQ_HPP
# define KQ_HPP

# include "Server.hpp"

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
        int getKq() const;
        std::vector<struct kevent>  getFdList() const;
        std::map<int, Server>   getServer() const;
        //logic
        void    plusEvent(uintptr_t fd, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata);
        void    plusClient(int serverFd);
        void    eventRead(struct kevent& store);
        void    eventWrite(struct kevent& store);
        void    mainLoop();
        //error
        void    clientError(struct kevent& store);
        void    serverError(struct kevent& store);
};

#endif
