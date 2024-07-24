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

#ifndef ENTITYLINE_HPP
# define ENTITYLINE_HPP

# include "HeaderLine.hpp"

class EntityLine
{
    private:
        bool                        completion;
        bool                        sizeEqual;
        int                         contentLength;
        std::string                 chunked;
        std::vector<std::string>    entity;
    public:
        //생성자 get고쳐야 함
        //occf
        EntityLine();
        EntityLine(const EntityLine& src);
        ~EntityLine();
        EntityLine& operator=(const EntityLine& src);
        //get function
        bool                        getCompletion() const;
        bool                        getSizeEqual() const;
        int                         getContentLength() const;
        std::string                 getChunked() const;
        std::vector<std::string>    getEntity() const;
        //logic
        void    initContentLength(int init);
        void    minusContentLength(int minus);
        int     chunkedEntity();
        int     plus(std::string &str, ENTITYTYPE entitytype);
};

#endif
