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
# include <map>
# include <sstream>
# include <algorithm>

enum    ENTITYTYPE 
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
        ENTITYTYPE  entitytype;
        int         contentLength;
        std::string key;
        std::string value;
        std::map<std::string, std::deque<std::string> > header;
        void    eraseSpace(std::string& str);
        // bool    checkMime(std::string temp);
        int     pushValue();
    public:
        //static variable
        static std::vector<std::string> manyHeader;
        static std::vector<std::string> vitalHeader;
        //occf
        HeaderLine();
        HeaderLine(const HeaderLine& src);
        ~HeaderLine();
        HeaderLine& operator=(const HeaderLine& src);
        //get function
        bool        getCompletion() const;
        TE          getTe() const;
        ENTITYTYPE  getEntitytype() const;
        int         getContentLength() const;
        std::string getKey() const;
        std::string getValue() const;
        std::map<std::string, std::deque<std::string> > getHeader() const;
        //set function
        void    setContentLength(int minus);
        void    setTrailer(TE temp);
        //logic
        int checkTrailer(std::string& temp);
        int plus(std::string& temp);
        int headerError();
};

#endif
