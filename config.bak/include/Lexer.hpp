/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 11:57:52 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/10 22:58:33 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __LEXER_HPP__
# define __LEXER_HPP__

#include <iostream>
#include <string>
#include <map>
#include <cctype>
#include <fstream>
#include <algorithm>
#include <sstream>
#include "Word.hpp"
#include "Tag.hpp"
#include "Num.hpp"
#include "UtilTemplate.hpp"

using namespace std;

class Token;

class Lexer {
private:
    string _fileName;
    class LexerException : public exception {
        private:
            string err;
        public:
            LexerException(string error, string fileName);
            ~LexerException() throw();
            const char *what() const throw();
    };
    void init();
public:
    static int line;
    static int column;
    static int startLine;
    static int startColumn;
    static char peek;
    string lookahead;
    ifstream file;
    map<string, Word> words;
    
    bool isTimeUnit(int c);
    bool isByteUnit(int c);
    bool isHTTPCode(int num);
    void lexerError(string error);
    void reserve(Word w);
    void checkNewLine();
    void readch();
    bool readch(char c);
    Lexer &operator=(const Lexer &l);
    Token *scan();
    string getFileName();
    Lexer();
    Lexer(const Lexer &l);
    Lexer(string fileName);
};

#endif