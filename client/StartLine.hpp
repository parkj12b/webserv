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

enum Method
{
    GET = 1,
    POST,
    HEAD,
    DELETE
};

enum Version
{
    HTTP10 = 1,
    HTTP11,
    HTTP12,
    HTTP20,
    HTTP30
};

class StartLine
{
    private:
        bool        completion;
        Method      method;
        Version     version;
        std::string url;
    public:
        static std::map<std::string, Method>  originMethod;
        static std::map<std::string, Version>  originVersion;
        StartLine();
        StartLine(const StartLine& src);
        StartLine&  operator=(const StartLine& src);
        ~StartLine();
        //get function
        bool        getCompletion() const;
        Method      getMethod() const;
        Version     getVersion() const;
        std::string getUrl() const;
        int plus(std::string tmp);
};

#endif
