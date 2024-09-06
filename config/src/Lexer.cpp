/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 20:03:36 by minsepar          #+#    #+#             */
/*   Updated: 2024/09/06 21:00:59 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lexer.hpp"

void    Lexer::init()
{
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
    // reserve(Word("allow", Tag::DIRECTIVE));
    reserve(Word("deny", Tag::DIRECTIVE));
    reserve(Word("GET", Tag::METHOD));
    reserve(Word("HEAD", Tag::METHOD));
    reserve(Word("POST", Tag::METHOD));
    reserve(Word("PUT", Tag::METHOD));
    reserve(Word("DELETE", Tag::METHOD));
    reserve(Word("CONNECT", Tag::METHOD));
    reserve(Word("OPTIONS", Tag::METHOD));
    reserve(Word("TRACE", Tag::METHOD));
    reserve(Word("~*", Tag::SYMBOL));
}

bool    Lexer::isTimeUnit(int c)
{
    return c == 's' || c == 'm' || c == 'h' || c == 'd';
}

bool    Lexer::isByteUnit(int c)
{
    return c == 'k' || c == 'm' || c == 'g' || c == 'K' || c == 'M' || c == 'G';
}

bool    Lexer::isHTTPCode(int num)
{
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

void    Lexer::lexerError(string error)
{
    throw LexerException(error, this);
}

void    Lexer::reserve(Word w)
{
    words.insert(make_pair(w.lexeme, w));
}

Lexer::Lexer() { init(); }

Lexer::Lexer(const Lexer &l) : words(l.words) {
    *this = l;
}

Lexer::Lexer(string fileName)
    : _fileName(fileName), line(1), column(0), startLine(1), startColumn(0),
        peek(' ')
{
    init();
    file.open(fileName);
    if (!file.is_open())
        std::exit(1);
}

void    Lexer::checkNewLine()
{
    if (peek == '\n') {
        line++;
        column = 0;
    }
}

string  Lexer::getFileName()
{
    return _fileName;
}

void    Lexer::readch()
{
    peek = file.get();
    column++;
}

bool    Lexer::readch(char c)
{
    readch();
    if (peek != c)
        return false;
    peek = ' ';
    return true;
}

Token *Lexer::scan() {
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
        LOG(cout << lookahead << endl);
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
                LOG(cout << "segment: " << segment << endl);
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
        // bool    isMask = false;
        do {
            // if (peek == '*')
            //     isMask = true;
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

Lexer &Lexer::operator=(const Lexer &l) {
    if (this != &l) {
        init();
        file.close();
        file.open(l._fileName);
        _fileName = l._fileName;
        line = l.line;
        column = l.column;
        startLine = l.startLine;
        startColumn = l.startColumn;
        peek = l.peek;
        lookahead = l.lookahead;
    }
    return *this;
}

Lexer::LexerException::LexerException(string error, Lexer *lex)
{
    err = lex->getFileName() + ":" + toString(lex->startLine) + ":"
        + toString(lex->startColumn) + ": error: " + error
        + getLineFromFile(lex->getFileName(), lex->startLine) + "\n"
        + getErrorAngle(lex->startColumn);
}

Lexer::LexerException::~LexerException() throw() {}

const char *Lexer::LexerException::what() const throw() { return err.c_str(); }

