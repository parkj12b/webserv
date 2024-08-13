/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inghwang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 18:57:00 by inghwang          #+#    #+#             */
/*   Updated: 2024/07/17 18:57:02 by inghwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADERLINE_HPP
# define HEADERLINE_HPP

# include <iostream>
# include <vector>
# include <deque>
# include <unordered_map>
# include <sstream>
# include <algorithm>
# include <cctype>

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

class HeaderLine
{
    private:
        bool        completion;
        TE          te;
        CONTENTTYPE contentType;
        int         port;
        size_t      contentLength;
        std::string key;
        std::string value;
        std::unordered_map<std::string, std::deque<std::string> > header;
        int     eraseSpace(std::string& str, bool space);
        // bool    checkMime(std::string temp);
        int     pushValue();
        int     commentDelete();
    public:
        //static variable
        static std::vector<std::string> singleHeader;
        static std::vector<std::string> dateHeader;
        static std::vector<std::string> vitalHeader;
        static std::vector<std::string> commentHeader;
        //Server, User-Agent, Via, comment
        //occf
        HeaderLine();
        HeaderLine(const HeaderLine& src);
        ~HeaderLine();
        HeaderLine& operator=(const HeaderLine& src);
        HeaderLine(int port);
        //get function
        bool        getCompletion() const;
        TE          getTe() const;
        CONTENTTYPE getContentType() const;
        int         getContentLength() const;
        std::string getKey() const;
        std::string getValue() const;
        std::unordered_map<std::string, std::deque<std::string> > getHeader() const;
        //set function
        void    setContentLength(int minus);
        void    setTrailer(TE temp);
        //logic
        int checkTrailer(std::string& temp);
        int plus(std::string& temp);
        int headerError();
};

#endif
