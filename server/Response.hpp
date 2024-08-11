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

typedef struct Request
{
    bool    fin;
    int     status;
    Method  method;
    Version version;
    std::string url;
    std::map<std::string, std::string>              query;
    std::map<std::string, std::deque<std::string> > header;
    std::vector<std::string>                        content;
}   Request;

class Response
{
    private:
        std::string start;
        std::string header;
        std::string content;
        std::string answer;
        Request     request;
    public:
        Response();
        Response(const Response& src);
        Response&    operator=(const Response& src);
        ~Response();
        //get function
        std::string getAnswer() const;
        //logic
        void    initRequest(Request temp);
        void    startLine();
        void    headerLine();
        void    contentLine();
        void    mainloop();
};

#endif
