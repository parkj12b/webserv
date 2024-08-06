/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 23:12:20 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/06 17:04:24 by minsepar         ###   ########.fr       */
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
    // static map<string, vector<
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
        Word *w = dynamic_cast<Word *>(_look);
        Env *newEnv = new Env(_top, w->lexeme);
        match(Tag::CONTEXT);
        headDirective(newEnv); match('{');
        directives();
        match('}');
    }

    void headDirective(Env *env) {
        vector<string> v = _top->getHeadDirective();
        string context = _top->getContext();
        int i;
        while (true) {
            i = 0;
            switch (_look->tag){
                //do type check here and add to vector
                case Tag::ID:
                case Tag::NUM:
            }
            Word *w = dynamic_cast<Word *>(_look);
            v.push_back(w->lexeme);
            match(Tag::ID);
            i++;
        }
        //args();
    }

    void head() {
        
    }

    void args() {
        
    }
};

#endif