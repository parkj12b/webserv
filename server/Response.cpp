/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inghwang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 16:25:04 by inghwang          #+#    #+#             */
/*   Updated: 2024/08/09 16:25:05 by inghwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response()
{}

Response::Response(const Response& src)
{
    answer = src.getAnswer();
}

Response&    Response::operator=(const Response& src)
{
    answer = src.getAnswer();
    return (*this);
}

Response::~Response()
{}

std::string Response::getAnswer() const
{
    return (answer);
}

void    Response::initRequest(Request temp)
{
    request = temp;
}

void    Response::startLine()
{

}

void    Response::headerLine()
{

}

void    Response::contentLine()
{

}

void    Response::mainloop()
{
    return ;
}
