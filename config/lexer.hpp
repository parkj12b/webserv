/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 11:57:52 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/07 02:07:50 by minsepar         ###   ########.fr       */
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

using namespace std;

bool isString(char peek);
string getLineFromFile(string filename, size_t lineNum);
string getErrorAngle(size_t column);

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
        ID = 256, IF = 257, IPV4 = 258, NUM = 259, DIRECTIVE = 260,
        CONTEXT = 261, VAARG = 263, MASK = 263, HTTPCODE = 264,
        BYTES = 265, TIME = 266, METHOD = 267;
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
        eq, minus, True, False, temp;
};

// = 

// const Word Word::eq = Word("=", Tag::EQ);

class Lexer {
private:
    string _fileName;
    class LexerException : public exception {
        private:
            string err;
        public:
            LexerException(string error, string fileName) {
                err = fileName + ":" + to_string(startLine) + ":"
                    + to_string(startColumn) + ": error: " + error + " [Lexer]\n"
                    + getLineFromFile(fileName, startLine) + "\n"
                    + getErrorAngle(startColumn);
            }
            ~LexerException() throw() {}
            const char *what() const throw() {
                return err.c_str();
            }
    };
    void init() {
        reserve(Word("events", Tag::CONTEXT));
        reserve(Word("http", Tag::CONTEXT));
        reserve(Word("server", Tag::CONTEXT));
        reserve(Word("location", Tag::CONTEXT));
        reserve(Word("limit_except", Tag::CONTEXT));
        reserve(Word("return", Tag::DIRECTIVE));
        reserve(Word("error_log", Tag::DIRECTIVE));
        reserve(Word("include", Tag::DIRECTIVE));
        reserve(Word("worker_connections", Tag::DIRECTIVE));
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
        reserve(Word("allow", Tag::DIRECTIVE));
        reserve(Word("deny", Tag::DIRECTIVE));
        reserve(Word("GET", Tag::METHOD));
        reserve(Word("HEAD", Tag::METHOD));
        reserve(Word("POST", Tag::METHOD));
        reserve(Word("PUT", Tag::METHOD));
        reserve(Word("DELETE", Tag::METHOD));
        reserve(Word("CONNECT", Tag::METHOD));
        reserve(Word("OPTIONS", Tag::METHOD));
        reserve(Word("TRACE", Tag::METHOD));
    }
public:
    static int line;
    static int column;
    static int startLine;
    static int startColumn;
    static char peek;
    string lookahead;
    ifstream file;
    map<string, Word> words;
    
    bool isTimeUnit(int c)
    {
        return c == 's' || c == 'm' || c == 'h' || c == 'd';
    }
    bool isByteUnit(int c)
    {
        return c == 'k' || c == 'm' || c == 'g' || c == 'K' || c == 'M' || c == 'G';
    }
    bool isHTTPCode(int num) {
        switch (num) {
            case 100:
            case 101:
            case 200:
            case 201:
            case 202:
            case 203:
            case 204:
            case 205:
            case 206:
            case 300:
            case 301:
            case 302:
            case 303:
            case 304:
            case 305:
            case 307:
            case 308:
            case 400:
            case 401:
            case 402:
            case 403:
            case 404:
            case 405:
            case 406:
            case 407:
            case 408:
            case 409:
            case 410:
            case 411:
            case 412:
            case 413:
            case 414:
            case 415:
            case 416:
            case 417:
            case 418:
            case 421:
            case 422:
            case 426:
            case 500:
            case 501:
            case 502:
            case 503:
            case 504:
            case 505:
                return true;
            default:
                return false;
        }
    }
    void lexerError(string error) {
        throw LexerException(error, _fileName);
    }
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
    Lexer(string fileName) : _fileName(fileName)
    {
        init();
        file.open(fileName);
        if (!file.is_open())
            throw runtime_error("File open fail");
    }
    void checkNewLine() {
        if (peek == '\n') {
            line++;
            column = 0;
        }
    }
    void readch() { peek = file.get();
        column++;
    }
    bool readch(char c) {
        readch();
        if (peek != c) return false;
        peek = ' ';
        return true;
    }
    Token *scan() {
        for (;; readch()) {
            if (peek == ' ' || peek == '\t')
            {
                startColumn = column;
                continue;
            }
            else if (peek == '\n'){
                line = line + 1;
                startLine = line;
                column = 0;
                startColumn = column;
            }
            else if (peek == '#') {
                while (peek != '\n') 
                    readch();
                line++;
                startLine = line;
                column = 0;
                startColumn = column;
            }
            else break;
        }
        if ( isdigit(peek) ) {
            string b = "";
            do
            {
                b += peek;
                readch();
            } while (isdigit(peek));
            // && (
            if (b.length() == 3)
            {
                int num = stoi(b);
                if (isspace(peek) && (lookahead == "return" || lookahead == "error_page"))
                {
                    if (isHTTPCode(num))
                        return new Word(b, Tag::HTTPCODE);
                }
            }
            if (isspace(peek) || peek == ';')
                return new Num(stoi(b));
            cout << lookahead << endl;
            if (lookahead == "client_max_body_size" && isByteUnit(peek))
            {
                b += peek;
                readch();
                if (isspace(peek) || peek == ';')
                    return new Word(b, Tag::BYTES);
            }
            if (lookahead == "keepalive_timeout" && isTimeUnit(peek))
            {
                b += peek;   
                readch();
                if (b[b.length() - 1] == 'm' && peek == 's')
                {
                    b += peek;
                    readch();
                }
                if (isspace(peek) || peek == ';')
                    return new Word(b, Tag::TIME);
            }
            if (lookahead == "listen" && peek == '.' && stoi(b) >= 0 && stoi(b) <= 255)
            {
                int curSegment = 2;
                b += peek;
                readch();
                while (true)
                {
                    string  segment = "";
                    int     segmentNum = 0;
                    while (isdigit(peek))
                    {
                        segment += peek;
                        readch();
                    }
                    b += segment;
                    cout << "segment: " << segment << endl;
                    segmentNum = stoi(segment);
                    if (segmentNum < 0 || segmentNum > 255)
                        break;
                    if (curSegment < 4 && peek != '.')
                        break;
                    b += peek;
                    readch();
                    if (curSegment == 4 && (isspace(peek) || peek == ';'))
                        return new Word(b, Tag::IPV4);
                    curSegment++;
                }
            }
            while (isString(peek) || isdigit(peek))
            {
                b += peek;
                readch();
            }
            return new Word(b, Tag::ID);
        }
        if (isString(peek)) {
            string  b = "";
            startLine = line;
            startColumn = column;
            bool    isMask = false;
            do {
                if (peek == '*')
                    isMask = true;
                if (peek == '[')
                {
                    startLine = line;
                    startColumn = column;
                    while (peek != ']')
                    {
                        if (file.eof() || peek == '\n')
                            lexerError("Incomplete Token: missing terminating ']' character");
                        b = b + peek;
                        readch();
                    }
                }
                b = b + peek;
                readch();
            } while (isString(peek));
            startLine = line;
            startColumn = column;
            map<string, Word>::iterator w = words.find(b);
            if (w != words.end()){
                if (w->second.tag == Tag::DIRECTIVE || w->second.tag == Tag::CONTEXT)
                    lookahead = b;                    
                return new Word(w->second);  
            }
            Word word = Word(b, Tag::ID);
            words.insert(make_pair(b, word));
            // words[b] = word;
            return new Word(word);
        }
        if (peek == '\"') {
            string b = "";
            startLine = line;
            startColumn = column;
            readch();
            while(peek != '\"') {
                if (file.eof() || peek == '\n')
                    lexerError("Incomplete Token: missing terminating '\"' character");
                b += peek;
                readch();
            }
            readch();
            return new Word(b, Tag::ID);
        }
        if (peek == ';' || peek == '{')
            lookahead = "";
        Token tok = Token(peek);
        peek = ' ';
        return new Token(tok);
    }
};

char Lexer::peek = ' ';
int Lexer::line = 1;
int Lexer::column = 0;
int Lexer::startLine = 1;
int Lexer::startColumn = 0;

string getLineFromFile(string filename, size_t lineNum)
{
    ifstream file(filename);
    string line;
    for (size_t i = 0; i < lineNum; i++)
        getline(file, line);
    return line;
}

string getErrorAngle(size_t column)
{
    string angle = "";
    for (size_t i = 1; i < column; i++)
        angle += " ";
    angle += "^";
    return angle;
}

#endif