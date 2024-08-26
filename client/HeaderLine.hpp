/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeaderLine.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 18:57:00 by inghwang          #+#    #+#             */
/*   Updated: 2024/08/13 18:24:35 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADERLINE_HPP
# define HEADERLINE_HPP

# include <iostream>
# include <vector>
# include <deque>
# include <map>
# include <sstream>
# include <algorithm>
# include <cctype>
# include "UtilTemplate.hpp"

enum    CONTENTTYPE 
{
    CONTENT = 1,
    TRANSFER,
    ENOT
};

enum    TE
{
    YES = 1,
    NOT
};

/**
 * @brief request message header make
 * @param completion HeaderLine completion status
 * @param connect keep-alive connect
 * @param te trailer status 
 * @param contentType content type(length or chunked)
 * @param port server port
 * @param contentLength content length size
 * @param key header key
 * @param value header value
 * @param header header vector
 * 
 */
class HeaderLine
{
    private:
        bool        completion;
        bool        connect;
        TE          te;
        CONTENTTYPE contentType;
        int         port;
        size_t      contentLength;
        std::string key;
        std::string value;
        std::map<std::string, std::deque<std::string> > header;
    public:
        //static variable
        static std::vector<std::string> singleHeader;   //only one header
        static std::vector<std::string> dateHeader;     //date header
        static std::vector<std::string> vitalHeader;    //vital header
        static std::vector<std::string> commentHeader;  //comment possible header
        //Server, User-Agent, Via, comment
        //occf
        HeaderLine();
        HeaderLine(const HeaderLine& src);
        ~HeaderLine();
        HeaderLine& operator=(const HeaderLine& src);
        HeaderLine(int port);
        //get function
        bool        getCompletion() const;
        bool        getConnect() const;
        TE          getTe() const;
        CONTENTTYPE getContentType() const;
        int         getPort() const;
        int         getContentLength() const;
        std::string getKey() const;
        std::string getValue() const;
        std::map<std::string, std::deque<std::string> > getHeader() const;
        //set function
        void    setContentLength(int minus);
        void    setTrailer(TE temp);
        //sub logic
        static int  eraseSpace(std::string& str, bool spel);    //front and back space delete spel do big changes small
        int     pushValue();                                //key -> value
        int     commentDelete();                            //comment delete
        //logic
        int parseTrailer(std::string& temp);                //trailer parsing
        int makeHeader(std::string& temp);                  //make header
        int headerError();                                  //header error
};

#endif
