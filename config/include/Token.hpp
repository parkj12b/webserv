/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 19:30:17 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/13 18:03:19 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __TOKEN_HPP__
# define __TOKEN_HPP__

#include <string>

using namespace std;

/**
 * @brief                       base token class
 * @param   tag                 tag of token
*/
class Token {
public:
    const int tag;
    Token operator=(const Token &t);
    string toString();
    virtual ~Token();
    Token(int t);
    Token(const Token &t);
    virtual Token *clone();
};

#endif