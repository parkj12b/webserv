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

# define AUTOINDEX_PATH "./cgi-bin/autoindex.py"

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
    std::vector<std::string>						content;    //request content
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
        //oocf
        Response();
        Response(const Response& src);
        Response&    operator=(const Response& src);
        ~Response();
        Response(int port);
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
        LocationConfigData *getLocationConfigData();
        //set function
        void        setRequest(Request &temp);
        void        setLocationConfigData(LocationConfigData *locationConfig);
		std::string setContent(string content_);
		void	    setContentLength(size_t contentLength_);
        //sub logic
		bool	isCgiScriptInURL(string& str);
        void    initRequest(Request msg);       //request msg init
        void    init();                         //start, header, content, entity init
        void    makeCookie(std::string& date);  //make cookie header
        void    makeDefaultHeader();            //date header make
        void    makeError();                    //error message make
        void    checkRedirect();                //check redirect
        void    checkAllowedMethod();           //check allowed method
        void    makeFilePath(std::string& str); //make real url
        int     getDefaultErrorPage(int statusCode); // returns fd of default error page
        void    makeHeader(std::string key, std::string value); //key -> value
        void    makeContent(int fd);                            //make content
        //logic
        void    makeEntity();               //plus start, header, content
        void    makeGet();                  //GET method make response
        void    makePost();                 //POST method make response
        void    makeDelete();               //DELETE method make response
        void    responseMake();             //request msg -> response msg
};

#endif
