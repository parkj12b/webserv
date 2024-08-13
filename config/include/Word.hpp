/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Word.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 19:52:40 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/13 18:05:55 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __WORD_HPP__
# define __WORD_HPP__

#include <string>
#include "Token.hpp"

using namespace std;

/**
 * @brief                       class used for storing word token
 * @param   lexeme              word string
 */
class Word : public Token {
public:
    string lexeme;
    Word(const Word& t);
    Word();
    Word(string s, int tag);
    string toString();
    Token *clone();
};

#endif