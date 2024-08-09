/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EntityLine.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inghwang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 16:24:14 by inghwang          #+#    #+#             */
/*   Updated: 2024/07/20 16:24:15 by inghwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTENTLINE_HPP
# define CONTENTLINE_HPP

# include "HeaderLine.hpp"

class ContentLine
{
    private:
        bool                        completion;
        bool                        sizeEqual;
        CONTENTTYPE                 contentType;
        int                         contentLength;
        std::string                 chunked;
        std::vector<std::string>    content;
    public:
        //생성자 get고쳐야 함
        //occf
        ContentLine();
        ContentLine(const ContentLine& src);
        ~ContentLine();
        ContentLine& operator=(const ContentLine& src);
        //get function
        bool                        getCompletion() const;
        bool                        getSizeEqual() const;
        int                         getContentLength() const;
        std::string                 getChunked() const;
        std::vector<std::string>    getContent() const;
        //logic
        void    initContentLine(int initCl, CONTENTTYPE initC);
        int     chunkedEntity();
        int     plus(std::string &str);
};

#endif
