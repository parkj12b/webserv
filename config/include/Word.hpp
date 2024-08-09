/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Word.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 19:52:40 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/09 21:53:16 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __WORD_HPP__
# define __WORD_HPP__

#include <string>
#include "Token.hpp"

using namespace std;

class Word : public Token {
public:
    string lexeme;
    Word(const Word& t);
    Word();
    Word(string s, int tag);
    string toString();
    const static Word
        eq, minus, True, False, temp;
};

#endif