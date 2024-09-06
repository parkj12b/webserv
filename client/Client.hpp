/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 17:11:17 by inghwang          #+#    #+#             */
/*   Updated: 2024/08/23 16:54:52 by minsepar         ###   ########.fr       */
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
        bool        connect;
        bool        connection;
        int         fd;
        int         port;
        int         socketReadSize;
        size_t      msgSize;
        size_t      index;
        size_t      responseAmount;
        ssize_t     standardTime;
        string		msg;
        Request     request;
        StartLine   startLine;
        HeaderLine  headerLine;
        ContentLine contentLine;
        Response    response;
		string		pathEnv;
        LocationConfigData
            *recurFindLocation(string url, LocationConfigData *locationConfigData);
    public:
		Client();
        Client(int fd, int port, string pathEnv_);
        explicit Client(const Client& src);
        Client& operator=(const Client& src);
        ~Client();
        //get function
        bool        getConnect() const;
        bool        getConnection() const;
        int         getFd() const;
        int         getPort() const;
        int         getSocketReadSize() const;
        size_t      getMsgSize() const;
        size_t      getIndex() const;
        size_t      getResponseAmount() const;
        ssize_t     getStandardTime() const;
        string		getMsg() const;
        time_t		getKeepAlive() const;
        Request     getRequest() const;
        StartLine   getStartLine() const;
        HeaderLine  getHeaderline() const;
        ContentLine getContentLine() const;
        Response    getResponse() const;
        Response    &getResponse();
        bool        getRequestFin() const;
        int         getRequestStatus() const;
        int         getResponseStatus() const;
        //set function
        void    setConnection(bool ycdi);
        void    setFd(uintptr_t fd);
        void    setRequestStatus(int temp);
        void    setRequestFin(bool fin);
        void    setCgiGetEntity(size_t &cgiContentLength, string &content);
		void	setCgiResponseEntity(size_t &cgiContentLength, string &content, size_t &status);
        void    plusSocketReadSize();
        //logic
        bool        getResponseCgi();
        void        deleteContent();
		void		clientIP(struct sockaddr_in clntAdr);
        size_t      responseIndex();    //response msg index(responseAmount - index)
        const char* respondMsgIndex();  //msg + index (const char*)
        int         setStart(void);     //startLine make
        int         setHeader(void);    //headerLine make
        int         setContent(void);   //contentLine make
        int         setTrailer(void);   //trailer make
        void        setMessage(const char* msgRequest, int &readSize); //request msg setting
        void        setResponseMessage();               //make response msg
        void        plusIndex(size_t plus);             //index plus
        void        resetClient();                      //reset client varidation
        bool        setMatchingLocation(string url);    // 임시
        // void    makeResponse();
        //temp(must delete)
        void        showMessage(void);  //request msg show
};

#endif
