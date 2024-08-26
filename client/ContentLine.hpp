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

# include <fstream> 
# include <fcntl.h>
# include <unistd.h>
# include <sys/stat.h>
# include "HeaderLine.hpp"
# include "UtilTemplate.hpp"

/**
 * @brief reqeust contentLine class
 * @param completion request contentline 완료 상태
 * @param contentType request contentType chunked OR length
 * @param port server port
 * @param contentLength contentType == length 길이 int값
 * @param chunked chunked msg save space
 * @param content length msg save space
 */
class ContentLine
{
    private:
        bool                        completion;
        CONTENTTYPE                 contentType;
        int                         fd;
        int                         port;
        int                         contentLength;
        size_t                      maxSize;
        std::string                 fileName;
        std::string                 chunked;
        std::vector<std::string>    content;
    public:
        //생성자 get고쳐야 함
        //occf
        ContentLine();
        ContentLine(const ContentLine& src);
        ~ContentLine();
        ContentLine(int port);
        ContentLine& operator=(const ContentLine& src);
        //get function
        bool                        getCompletion() const;
        CONTENTTYPE                 getContentType() const;
        int                         getFd() const;
        int                         getPort() const;
        int                         getContentLength() const;
        size_t                      getMaxSize() const;
        std::string                 getFileName() const;
        std::string                 getChunked() const;
        std::vector<std::string>    getContent() const;
        //logic
        bool    fileExist(const char *fileName_);   //tempFileName exist
        bool    tempFileMake(int &num); //temp file make
        void    initContentLine(int initLength, CONTENTTYPE initType);  //init
        int     chunkedEntity();                    //chunked message parsing
        int     makeContentLine(std::string &str, size_t &readSize, int &status);  //contentLine make
};

#endif
