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
# include "Response.hpp"
# include "Server.hpp"
# include <unistd.h>
// # include <iostream> 
// # include <map>
# include <queue>
# include <fstream>
# include <sys/socket.h>

//delete
# include <ctime>

//뭔가 생성이 될 때에 소멸자가 생성이 되는 느낌?? 왜 일까??

/**
 * @brief client class로 request, respond message를 만들어냄
 * @param fd client fd
 * @param port server port
 * @param index msg read index
 * @param responseAmount msg total size
 * @param request request msg
 * @param startLine request startLine
 * @param headerLine request headerLine
 * @param contentLine request contentLine
 * @param response response msg
 */
class Client
{
    private:
        int         fd;
        int         port;
        size_t      index;
        size_t      responseAmount;
        std::string msg;
        Request     request;
        StartLine   startLine;
        HeaderLine  headerLine;
        ContentLine contentLine;
        Response    response;
        //temp(must delete)
    public:
        Client();
        Client(int fd, int port);
        explicit Client(const Client& src);
        Client& operator=(const Client& src);
        ~Client();
        //get function
        int         getFd() const;
        int         getPort() const;
        size_t      getIndex() const;
        size_t      getResponseAmount() const;
        std::string getMsg() const;
        Request     getRequest() const;
        StartLine   getStartLine() const;
        HeaderLine  getHeaderline() const;
        ContentLine getContentLine() const;
        Response    getResponse() const;
        bool        getRequestFin() const;
        int         getRequestStatus() const;
        //set function
        void    setFd(uintptr_t fd);
        void    setRequestStatus(int temp);
        //logic
        size_t      responseIndex();    //response msg index(responseAmount - index)
        const char* respondMsgIndex();  //msg + index (const char*)
        int         setStart(void);     //startLine make
        int         setHeader(void);    //headerLine make
        int         setContent(void);   //contentLine make
        int         setTrailer(void);   //trailer make
        void        setMessage(std::string msgRequest); //request msg setting
        void        setResponseMessage();               //make response msg
        void        plusIndex(size_t plus);             //index plus
        // void    makeResponse();
        //temp(must delete)
        void        showMessage(void);  //request msg show
};

#endif
