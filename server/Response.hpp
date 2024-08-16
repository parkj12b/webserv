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

#ifndef RESONSE_HPP
# define RESONSE_HPP

# include <iostream>
# include <vector>
# include <deque>
# include <fstream>
# include <unistd.h>
# include <fcntl.h>
# include <cstdio>
# include "StartLine.hpp"
# include "ServerConfigData.hpp"

using namespace std;

typedef struct Request
{
    bool    fin;    //request completion status
    int     status; //request status code
    Method                                                      method;     //http method
    std::string                                                 url;        //http resource
    std::string                                                 location;   //location for config
    Version                                                     version;    //http version
    std::unordered_map<std::string, std::string>                query;      //resource query
    std::unordered_map<std::string, std::deque<std::string> >   header;     //request header
    std::vector<std::string>                                    content;    //request content
}   Request;

/**
 * @brief           request message
 * @param port      server port
 * @param path      server path
 * @param location  location block path
 * @param start     response startline
 * @param header    response header
 * @param content   response content
 * @param entity    response messgae entity
 * @param request   request struct
 * @param serverConfig    server used to take care of request.
 */
class Response
{
    private:
        int                 port;
        std::string         start;
        std::string         header;
        std::string         content;
        std::string         entity;
        Request             request;
        ServerConfigData    *serverConfig;    //server config
        void        makeFilePath(std::string& str);
    public:
        static std::map<int, std::string>   statusContent;
        //oocf
        Response();
        Response(const Response& src);
        Response&    operator=(const Response& src);
        ~Response();
        Response(int port);
        //get function
        int         getPort() const;
        std::string getStart() const;
        std::string getHeader() const;
        std::string getContent() const;
        std::string getEntity() const;
        Request     getRequest() const;
        //set function
        void    setRequest(Request &temp);
        //logic
        void    init();         //start, header, content, entity init
        int     getDefaultErrorPage(int statusCode); // returns fd of default error page
        void    makeDate();     //date header make
        void    makeError();    //error message make
        void    makeHeader(std::string key, std::string value); //key -> value
        void    makeContent(int fd);        //make content
        void    makeEntity();               //plus start, header, content
        void    makeGet();                  //GET method make response
        void    makePost();                 //POST method make response
        void    makeDelete();               //DELETE method make response
        void    initRequest(Request msg);   //request msg init
        void    responseMake();             //request msg -> response msg
        void    checkAllowedMethod();       //check allowed method
};

#endif
