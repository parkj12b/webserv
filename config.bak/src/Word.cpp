/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Word.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 19:55:11 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/10 20:35:16 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Word.hpp"

using namespace std;

string Word::toString() { return lexeme; }

Token *Word::clone() { return new Word(*dynamic_cast<Word *>(this)); }

Word::Word(const Word& t)
    : Token(t.tag), lexeme(t.lexeme) {}

Word::Word() : Token(0), lexeme("") {}

Word::Word(string s, int tag) : Token(tag), lexeme(s) {}