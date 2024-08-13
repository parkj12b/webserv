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
# include "../client/StartLine.hpp"
# include <vector>
# include <deque>
# include <fstream>
# include <unistd.h>
# include <fcntl.h>
# include <cstdio>

typedef struct Request
{
    bool    fin;
    int     status;
    Method                                                      method;
    std::string                                                 url;
    Version                                                     version;
    std::unordered_map<std::string, std::string>                query;
    std::unordered_map<std::string, std::deque<std::string> >   header;
    std::vector<std::string>                                    content;
}   Request;

class Response
{
    private:
        std::string start;
        std::string header;
        std::string content;
        std::string entity;
        Request     request;
        void        makeFilePath(std::string& str);
    public:
        static std::map<int, std::string>   statusContent;
        Response();
        Response(const Response& src);
        Response&    operator=(const Response& src);
        ~Response();
        //get function
        std::string getStart() const;
        std::string getHeader() const;
        std::string getContent() const;
        std::string getEntity() const;
        Request     getRequest() const;
        //set function
        void    setRequest(Request &temp);
        //logic
        void    init();
        void    makeDate();
        void    makeError();
        void    makeHeader(std::string key, std::string value);
        void    makeContent(int fd);
        void    makeEntity();
        void    makeGet();
        void    makePost();
        void    makeDelete();
        void    initRequest(Request temp);
        void    responseMake();
};

#endif
