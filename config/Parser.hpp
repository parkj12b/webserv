/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 23:12:20 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/02 17:20:43 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __PARSER_HPP__
# define __PARSER_HPP__

#include "Lexer.hpp"
#include "Env.hpp"

class Parser {
private:
    Lexer _lex;
    Token *_look;
    Env *_top;
    int _used = 0;
    string _context;
public:
    Parser(Lexer l, string context)
        : _lex(l), _context(context) { move(); }
    void move() { _look = _lex.scan(); }
    void error(string s) { throw runtime_error("near line " + to_string(_lex.line) + ": " + s); }
    void match(int t) {
        if (_look->tag == t) move();
        else error("syntax error");
    }
    void program() {
        switch (_look->tag) {
            case Tag::DIRECTIVE:
                directives();
                break;
            case Tag::CONTEXT:
                context();
                break;
        }
    }

    void directives() {
        switch(_look->tag) {
            case Tag::DIRECTIVE:
                directive(); directives();
                break;
            case Tag::CONTEXT:
                context(); directives();
                break;
        }
    }

    void directive() {
        head(); args(); match(';');
    }

    void context() {
        Env *newEnv = new Env(_top);
        headDirective(newEnv); match('{');
        directives();
        match('}');
    }

    void headDirective(Env *env) {
        vector<string> v = _top->getHeadDirective();
        Word *w = dynamic_cast<Word *>(_look);
        string context = w->lexeme;
        match(Tag::CONTEXT);
        while (true) {
            switch (_look->tag){
                case Tag::ID:
                case Tag::NUM:
                case Tag::REAL:
            }
            Word *w = dynamic_cast<Word *>(_look);
            v.push_back(w->lexeme);
            match(Tag::ID);
        }
        args();
    }

    void head() {
        
    }

    void args() {
        
    }
};

#endif