/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inghwang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 17:11:17 by inghwang          #+#    #+#             */
/*   Updated: 2024/07/08 17:11:19 by inghwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "StartLine.hpp"
# include "HeaderLine.hpp"
# include "ContentLine.hpp"
# include <unistd.h>
// # include <iostream> 
// # include <map>
# include <queue>

//delete
# include <ctime>

//뭔가 생성이 될 때에 소멸자가 생성이 되는 느낌?? 왜 일까??


typedef struct Request
{
    bool    fin;
    int     status;
    Method  method;
    Version version;
    std::string url;
    std::map<std::string, std::deque<std::string> >  header;
    std::map<std::string, std::deque<std::string> >  trailer;
    std::vector<std::string>    entity;
}   Request;

class Client
{
    private:
        int         fd;
        std::string msg;
        Request     request;
        StartLine   startLine;
        HeaderLine  headerLine;
        ContentLine contentLine;
        //temp(must delete)
    public:
        Client();
        Client(int fd);
        explicit Client(const Client& src);
        Client& operator=(const Client& src);
        ~Client();
        //get function
        int                         getFd() const;
        std::string                 getMsg() const;
        Request                     getRequest() const;
        StartLine                   getStartLine() const;
        HeaderLine                  getHeaderline() const;
        ContentLine                 getContentLine() const;
        bool                        getRequestFin() const;
        int                         getRequestStatus() const;
        //set function
        void    setFd(uintptr_t fd);
        void    setRequestStatus(int temp);
        int     setStart(void);
        int     setHeader(void);
        int     setContent(void);
        int     setTrailer(void);
        //logic
        void    setMessage(std::string str);
        bool    getRequestFin();
        //temp(must delete)
        void    showMessage(void);
};

#endif
