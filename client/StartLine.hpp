/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Startline.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inghwang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 12:55:26 by inghwang          #+#    #+#             */
/*   Updated: 2024/07/17 12:55:28 by inghwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STARTLINE_HPP
# define STARTLINE_HPP

# include <iostream>
# include <sstream>
# include <map>
# include <unordered_map>

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

class StartLine
{
    private:
        bool                                            completion;
        Version                                         version;
        Method                                          method;
        int                                             port;
        std::string                                     url;
        std::unordered_map<std::string, std::string>    query;
    public:
        //static variable
        static std::map<std::string, Method>    originMethod;
        static std::map<std::string, Version>   originVersion;
        //occf
        StartLine();
        StartLine(const StartLine& src);
        StartLine&  operator=(const StartLine& src);
        ~StartLine();
        StartLine(int port);
        //get function
        bool                                            getCompletion() const;
        Method                                          getMethod() const;
        Version                                         getVersion() const;
        int                                             getPort() const;
        std::string                                     getUrl() const;
        std::unordered_map<std::string, std::string>    getQuery() const;
        //sub logic
        int urlQuery();
        //logic
        int plus(std::string tmp);
};

#endif
