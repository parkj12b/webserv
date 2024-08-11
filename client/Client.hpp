/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 17:11:17 by inghwang          #+#    #+#             */
/*   Updated: 2024/08/11 14:45:08 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "StartLine.hpp"
# include "HeaderLine.hpp"
# include "ContentLine.hpp"
// # include "Response.hpp"
# include <unistd.h>
// # include <iostream> 
// # include <map>
# include <queue>
# include <fstream>

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
    std::unordered_map<std::string, std::string>                query;
    std::unordered_map<std::string, std::deque<std::string> >   header;
    std::vector<std::string>                                    content;
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
        // Response     response;
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
        bool    getRequestFin();
        void    setMessage(std::string str);
        // void    makeResponse();
        //temp(must delete)
        void    showMessage(void);
};

#endif
