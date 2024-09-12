/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StartLine.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 12:55:26 by inghwang          #+#    #+#             */
/*   Updated: 2024/08/15 15:15:45 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STARTLINE_HPP
# define STARTLINE_HPP

# include <iostream>
# include <sstream>
# include <map>

using namespace std;

enum Method
{
    GET = 1,
    POST,
    DELETE
};

enum Version
{
    HTTP11 = 1
};

/**
 * @brief               request message startline class
 * @param completion    startline status
 * @param version       http version
 * @param method        http possible method
 * @param port          server port
 * @param url           resource
 * @param query         all query
 */
class StartLine
{
    private:
        bool                                            completion;
        Version                                         version;
        Method                                          method;
        int                                             port;
        std::string                                     url;
        std::string                                     location;
        std::map<std::string, std::string>    query;
    public:
        //static variable
        static std::map<std::string, Method>    originMethod;   //possible method
        static std::string                      methodString[4];
        static std::map<std::string, Version>   originVersion;  //possible version
        //occf
        StartLine();
        StartLine(const StartLine& src);
        StartLine&  operator=(const StartLine& src);
        ~StartLine();
        StartLine(int port);
        //get function
        bool                            getCompletion() const;
        Method                          getMethod() const;
        Version                         getVersion() const;
        int                             getPort() const;
        string                          getUrl() const;
        std::string                     getLocation() const;    
        map<string, string>   getQuery() const;
        //sub logic
        void                             urlQuery(); //query parsing
        //logic
        int check(std::string firstLine);   //startline make
};

#endif
