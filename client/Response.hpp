/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inghwang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 16:25:07 by inghwang          #+#    #+#             */
/*   Updated: 2024/08/09 16:25:10 by inghwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <algorithm>
# include <iostream>
# include <vector>
# include <deque>
# include <fstream>
# include <unistd.h>
# include <fcntl.h>
# include <cstdio>
# include "ServerConfigData.hpp"
# include "StartLine.hpp"
# include "CgiProcessor.hpp"
# include "HeaderLine.hpp"

# define AUTOINDEX_PATH "/cgi-bin/autoindex.py"

using namespace std;

typedef struct Request
{
    bool    fin;    //request completion status
    int     status; //request status code
    int     port;		//server port
	int		clientFd;	//client File Descriptor
    Method											method;     //http method
	std::string										clientIp;	//client IP
    std::string										url;        //http resource
    std::string										location;   //location for config
    Version											version;    //http version
    std::map<std::string, std::string>				query;      //resource query
    std::map<std::string, std::deque<std::string> >	header;     //request header
    std::string						                contentFileName;    //request content
}   Request;

class Server;

/**
 * @brief           request message
 * @param cgiFlag   server cgi flag
 * @param port      server port
 * @param contentLength response content length
 * @param path      server path
 * @param location  location block path
 * @param start     response startline
 * @param header    response header
 * @param content   response content
 * @param entity    response messgae entity
 * @param request   request struct
 * @param serverConfig    server used to take care of request.
 * @param locationConfig  location used to take care of request.
 */
class Response
{
    private:
		bool				cgiFlag;
        int                 port;
        size_t              startHeaderLength;
		size_t				contentLength;
		string				pathEnv;
        std::string         start;
        std::string         header;
        std::string         content;
        std::string         entity;
        Request             request;
        std::vector<std::string>    keyHeader;
        ServerConfigData    *serverConfig;    //server config
        LocationConfigData  *locationConfig;  //location config
    public:
        static std::map<int, std::string>           statusContent;
        static std::map<std::string, std::string>   session;
        static std::vector<std::string>             cgiHeader;
        static std::map<std::string, std::string>   urlContentType;
        //oocf
        Response();
        Response(const Response& src);
        Response&    operator=(const Response& src);
        ~Response();
        Response(int port, string pathEnv_);
        //get function
        int         getPort() const;
        size_t      getStartHeaderLength() const;
        size_t      getContentLength() const;
        std::string getStart() const;
        std::string getHeader() const;
        std::string getContent() const;
        std::string getEntity() const;
        Request     getRequest() const;
        int         getRequestStatus() const;
		bool		getCgiFlag() const;
        std::vector<std::string>    getKeyHeader() const;
        LocationConfigData *getLocationConfigData() const;
        //set function
        void        setCgiFlag(bool flag);
        void        setPort(int port);
		void		setPathEnv(string pathEnv_);
        void        setRequest(Request &temp);
        void        setRequestStatus(int status);
        void        setLocationConfigData(LocationConfigData *locationConfig);
        size_t      setCgiHeader(string &content_, size_t &status);
        void        setCgiGetContent(string &content_);
		void	    setCgiGetHeader(size_t &contentLength_);
		size_t      setCgiContent(string &content_, size_t &status);
		void	    setCgiContentLength(size_t contentLength_);
        //sub logic
		bool	isCgiScriptInURL(string& str);
        void    initRequest(Request msg);       //request msg init
        bool    init();                         //start, header, content, entity init
        void    makeCookie(std::string& date);  //make cookie header
        void    makeDefaultHeader();            //date header make
        void    makeError();                    //error message make
        int     checkRedirect();                //check redirect
        int     checkAllowedMethod();           //check allowed method
        void    makeFilePath(std::string& str); //make real url
        void    makeHeader(std::string key, std::string value); //key -> value
        void    makeContent(int fd);                            //make content
        bool    isValidUploadPath();                            //check upload path
        //logic
        void    makeEntity();               //plus start, header, content
        void    makeGet();                  //GET method make response
        void    makePost();                 //POST method make response
        void    makeDelete();               //DELETE method make response
        void    responseMake();             //request msg -> response msg
};

#endif
