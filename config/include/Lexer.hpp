/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 11:57:52 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/13 16:25:18 by minsepar         ###   ########.fr       */
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

/**
 * @brief                   Lexer class
 * @param   line            current line number
 * @param   column          current column number
 * @param   startLine       start line number, first pointer in
 *                          two pointer representation
 * @param   startColumn     start column number
 * @param   peek            current character
 * @param   lookahead       current context / directive token
 * @param   file            file stream
 * @param   words           reserved words to determine token type
*/
class Lexer {
private:
    class LexerException : public exception {
        private:
            string err;
        public:
            LexerException(string error, Lexer *lex);
            ~LexerException() throw();
            const char *what() const throw();
    };
    string  _fileName;
    void    init();
public:
    //왜 이것들을 public하게 선언하였는지
    int                 line;
    int                 column;
    int                 startLine;
    int                 startColumn;
    char                peek;
    string              lookahead;
    ifstream            file;
    map<string, Word>   words;
    
    string  getFileName();
    bool    isTimeUnit(int c);
    bool    isByteUnit(int c);
    bool    isHTTPCode(int num);
    void    lexerError(string error);

    /* add reserve words in words table */
    void    reserve(Word w);

    /* new line check helper function */
    void    checkNewLine();

    /* read a single char and save it in peek */
    void    readch();
    bool    readch(char c);

    /* lexer token scan logic */
    Token   *scan();

    Lexer   &operator=(const Lexer &l);
    Lexer();
    Lexer(const Lexer &l);
    Lexer(string fileName);
};

#endif