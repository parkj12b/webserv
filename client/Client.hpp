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
# include "EntityLine.hpp"
# include <unistd.h>
// # include <iostream> 
// # include <map>
# include <queue>

//delete
# include <ctime>

typedef struct Request
{
    bool    fin;
    Method  method;
    Version version;
    std::string url;
    std::map<std::string, std::deque<std::string> >  header;
    std::vector<std::string>    entity;
}   Request;

class Client
{
    private:
        int         fd;
        std::string msg;
        Request     request;
        StartLine   startline;
        HeaderLine  headerline;
        EntityLine  entityline;
        //temp(must delete)
        std::queue<std::string>     message;
    public:
        Client();
        explicit Client(const Client& src);
        Client& operator=(const Client& src);
        ~Client();
        Client(int fd);
        //get function
        int                         getFd() const;
        std::string                 getMsg() const;
        Request                     getRequest() const;
        StartLine                   getStartLine() const;
        HeaderLine                  getHeaderline() const;
        EntityLine                  getEntity() const;
        std::queue<std::string>     getMessage() const;
        bool                        getRequestFin() const;
        //set function
        void    setFd(int fd);
        int     setStartLine(void);
        int     setHeaderUtil(std::string temp);
        int     setHeader(void);
        int     setEntityLine(void);
        int     setTe(void);
        //temp(must delete)
        void    showMessage(void);
        void    setMessage(std::string str);
};

#endif
