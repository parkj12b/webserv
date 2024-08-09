/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 19:30:53 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/09 22:05:35 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Token.hpp"
#include "UtilTemplate.hpp"

Token Token::operator=(const Token &t)
{
    (void) t;
    return *this;
}

string Token::toString() { return intToUtf8(tag); }

Token::~Token() {}

Token::Token(int t) : tag(t) {}

Token::Token(const Token &t) : tag(t.tag) {}
