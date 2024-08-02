/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 11:57:52 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/02 17:20:30 by minsepar         ###   ########.fr       */
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

using namespace std;

std::string intToUtf8(int codePoint) {
    std::string utf8String;

    if (codePoint < 0 || codePoint > 0x10FFFF) {
        throw std::out_of_range("Code point out of range for Unicode");
    }

    if (codePoint <= 0x7F) {
        // 1-byte UTF-8
        utf8String += static_cast<char>(codePoint);
    } else if (codePoint <= 0x7FF) {
        // 2-byte UTF-8
        utf8String += static_cast<char>(0xC0 | (codePoint >> 6));
        utf8String += static_cast<char>(0x80 | (codePoint & 0x3F));
    } else if (codePoint <= 0xFFFF) {
        // 3-byte UTF-8
        utf8String += static_cast<char>(0xE0 | (codePoint >> 12));
        utf8String += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
        utf8String += static_cast<char>(0x80 | (codePoint & 0x3F));
    } else {
        // 4-byte UTF-8
        utf8String += static_cast<char>(0xF0 | (codePoint >> 18));
        utf8String += static_cast<char>(0x80 | ((codePoint >> 12) & 0x3F));
        utf8String += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
        utf8String += static_cast<char>(0x80 | (codePoint & 0x3F));
    }

    return utf8String;
}

class Tag {
public:
    static const int
        AND = 256, BASIC = 257, BREAK = 258, DO = 259, ELSE = 260,
        FALSE = 262, ID = 264, IF = 265, INDEX = 266,
        MINUS = 268, NUM = 270, REAL = 272, TEMP = 273,
        TRUE = 274, WHILE = 275, DIRECTIVE = 277,
        CONTEXT = 278;
};

class Token {
public:
    const int tag;
    virtual ~Token() {}
    Token(int t) : tag(t) {}
    Token(const Token &t) : tag(t.tag) {}
    Token operator=(const Token &t) { (void) t; return *this; }
    string toString() { return intToUtf8(tag); }
};

class Num : public Token {
public:
    const int value;
    Num(int v) : Token(Tag::NUM), value(v) {}
    string toString() { return to_string(value); }
};

class Word : public Token {
public:
    string lexeme;
    Word(const Word& t)
        : Token(t.tag), lexeme(t.lexeme) {}
    // Word &operator=(const Word &t) {
    //     if (this != &t) {
    //         tag = t.tag;
    //         lexeme = t.lexeme;
    //     }
    //     return *this;
    // }
    Word() : Token(0), lexeme("") {}
    Word(string s, int tag) : Token(tag), lexeme(s) {}
    string toString() { return lexeme; }
    const static Word
        minus, True, False, temp;
};

class Real : public Token {
public:
    const float value;
    Real(float v) : Token(Tag::REAL), value(v) {}
    string toString() { return to_string(value); }
};

class Lexer {
private:
    void init() {
        reserve(Word("events", Tag::CONTEXT));
        reserve(Word("http", Tag::CONTEXT));
        reserve(Word("server", Tag::CONTEXT));
        reserve(Word("location", Tag::CONTEXT));
        reserve(Word("limit_except", Tag::CONTEXT));
        reserve(Word("error_log", Tag::DIRECTIVE));
        reserve(Word("include", Tag::DIRECTIVE));
        reserve(Word("worker_connection", Tag::DIRECTIVE));
        reserve(Word("default_type", Tag::DIRECTIVE));
        reserve(Word("keepalive_timeout", Tag::DIRECTIVE));
        reserve(Word("listen", Tag::DIRECTIVE));
        reserve(Word("server_name", Tag::DIRECTIVE));
        reserve(Word("root", Tag::DIRECTIVE));
        reserve(Word("error_page", Tag::DIRECTIVE));
        reserve(Word("client_max_body_size", Tag::DIRECTIVE));
        reserve(Word("fastcgi_pass", Tag::DIRECTIVE));
        reserve(Word("fastcgi_index", Tag::DIRECTIVE));
        reserve(Word("fastcgi_param", Tag::DIRECTIVE));
        reserve(Word("index", Tag::DIRECTIVE));
        reserve(Word("autoindex", Tag::DIRECTIVE));
        reserve(Word("log_format", Tag::DIRECTIVE));
        reserve(Word::True);
        reserve(Word::False);
        reserve(Word::temp);
    }
public:
    static int line;
    static char peek;
    ifstream file;

    map<string, Word> words;
    void reserve(Word w) {
        words.insert(make_pair(w.lexeme, w));
        // words[w.lexeme] = w;
        // cout<<w.lexeme<<endl;
    }
    Lexer() {
        init();
        // reserve(Type::Int);
        // reserve(Type::Char);
        // reserve(Type::Bool);
        // reserve(Type::Float);
    }
    Lexer(const Lexer &l) : words(l.words) {}
    Lexer(string fileName)
    {
        init();
        file.open(fileName);
        if (!file.is_open())
            throw runtime_error("File open fail");
    }
    void readch() { peek = file.get(); }
    bool readch(char c) {
        readch();
        if (peek != c) return false;
        peek = ' ';
        return true;
    }
    Token *scan() {
        for (;; readch()) {
            if (peek == ' ' || peek == '\t') continue;
            else if (peek == '\n') line = line + 1;
            else if (peek == '#') {
                while (peek != '\n') 
                    readch();
            }
            else break;
        }
        if ( isdigit(peek) ) {
            int v = 0;
            do {
                v = 10 * v + (peek - '0');
                readch();
            } while ( isdigit(peek) );
            
            if (peek != '.')
                return new Num(v);  
            float x = v, d = 10;
            for(;;) {
                readch();
                if (!isdigit(peek)) break;
                x = x + (peek - '0') / d;
                d = d * 10;
            }
            return new Real(x);
        }
        if (isalpha(peek)) {
            string b = "";
            do {
                b = b + peek;
                readch();
            } while (isalnum(peek) || peek == '_' || peek == '.' || peek == '-'
                || peek == '/' || peek == ':' || peek == '=');
            map<string, Word>::iterator w = words.find(b);
            if (w != words.end()){
                // cout << "str: " << b << endl;
                // cout << "found: " << w->second.lexeme << endl;
                return new Word(w->second);  
            }
            Word word = Word(b, Tag::ID);
            words.insert(make_pair(b, word));
            // words[b] = word;
            return new Word(word);
        }
        if (peek == '\"') {
            string b = "";
            readch();
            do {
                b = b + peek;
                readch();
            } while (peek != '\"');
            return new Word(b, Tag::ID);
        }
        
        Token tok = Token(peek);
        peek = ' ';
        return new Token(tok);
    }
};

char Lexer::peek = ' ';
int Lexer::line = 1;


#endif