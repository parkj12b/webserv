/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 18:48:59 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/09 22:04:50 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "Token.hpp"
#include "Env.hpp"
#include "Syntax.hpp"
#include "Tag.hpp"
#include "Word.hpp"
#include "ServerConfig.hpp"
#include "LocationConfig.hpp"
#include "Directives.hpp"
#include "Word.hpp"

Parser::Parser(Lexer &l, string context)
    : _lex(l), _top(new Env(NULL, context)), _event(NULL) { move(); }

Env *Parser::getEvent() { return _top; }

int Parser::getDirectiveNum(string s) { return _directiveNum[s]; }

vector<ServerConfig *> Parser::getServerConfig() { return _serverConfig; }

void Parser::move() { _look = _lex.scan(); }

void Parser::match(int t) {
    if (_look->tag == t) move();
    else error("syntax error");
}

void Parser::program() { directives(); }

void Parser::directives()
{
    switch(_look->tag) {
        case Tag::DIRECTIVE:
            directive(); directives();
            break;
        case Tag::CONTEXT:
            context(); directives();
            break;
    }
}

void Parser::directive()
{
    Word *w = dynamic_cast<Word *>(_look);
    string context = w->lexeme;
    move();
    vector<Syntax> syntaxList = _directiveSyntax[context];
    vector<vector< Token *> > v;
    size_t i = 0;
    while (_look->tag != ';' || _look->tag == -1) {
        vector< Token *> subV;
        if (i < v.size())
            subV = v[i];
        Syntax s = syntaxList[i];
        vector<int> tag = s.tag;
        bool isMatched = false;
        for (size_t j = 0; j < tag.size(); j++)
        {
            if (_look->tag == tag[j]) {
                isMatched = true;
                while (i >= v.size())
                    v.push_back(vector<Token *>());
                v[i].push_back(_look);
                move();
                break;
            }
        }
        if (!isMatched) {
            if (s.required == 1) error("syntax error");
        }
        else {
            if (s.required == 2) i--;
        }
        i++;
    }
    _top->put(context, v);
    match(';');
}

void    Parser::server()
{
    curServer = new ServerConfig(_top);
    cout << "in server" << endl;
    _serverConfig.push_back(curServer);
}

void    Parser::context()
{
    cout << "in context" << endl;
    Word *w = dynamic_cast<Word *>(_look);
    cout << _top->getContext() << endl;
    if (!Directives::containsContext(_top->getContext(), w->lexeme))
        error("invalid context");
    Env *temp = _top;
    _top = new Env(_top, w->lexeme);
    if (_directiveNum[w->lexeme] == SERVER)
        server();
    match(Tag::CONTEXT);
    headDirective(); match('{');
    directives();
    match('}');
    Token *t = NULL;
    string path;
    switch(_directiveNum[w->lexeme]) {
        case EVENTS:
            if (_event != NULL)
                error("multiple events");
            _event = _top;
            break;
        case LOCATION:
            t = _top->getHeadDirectiveByIndex(1)[0];
            path = dynamic_cast<Word *>(t)->lexeme;
            cout << path << endl;
            if (curServer->location.find(path) == curServer->location.end())
                curServer->location.insert(make_pair(path, LocationConfig(_top)));
            break;
        case LIMIT_EXCEPT:
            cout << "size: " << _top->getPrev()->getHeadDirectiveByIndex(1).size() << endl;
            t = _top->getPrev()->getHeadDirectiveByIndex(1)[0];
            path = dynamic_cast<Word *>(t)->lexeme;
            if (curServer->location.find(path) != curServer->location.end()
                && curServer->location.find(path)->second.getLimitExcept() != NULL)
                error("multiple limit_except");
            curServer->location.insert(make_pair(path, LocationConfig(_top->getPrev(), _top)));
            curServer->location[path].setLimitExcept(_top);
            break;
    }
    _top = temp;
}

void    Parser::headDirective()
{
    cout << "in headDirective" << endl;
    string context = _top->getContext();
    vector<Syntax> syntaxList = _directiveSyntax[context];
    int i = 0;
    while (_look->tag != '{') {
        Syntax s = syntaxList[i];
        vector<Token *> &v = _top->getHeadDirectiveByIndex(i);
        vector<int> tag = s.tag;
        bool isMatched = false;
        for (size_t j = 0; j < tag.size(); j++)
        {
            if (_look->tag == tag[j]) {
                isMatched = true;
                v.push_back(_look);
                move();
                break;
            }
        }
        if (!isMatched) {
            if (s.required == 1) error("syntax error");
        }
        else {
            if (s.required == 2) i--;
        }
        i++;
    }
}

map<string, int> Parser::_directiveNum = {
    {"error_log", 1},
    {"worker_connections", 2},
    {"default_type", 3},
    {"keepalive_timeout", 4},
    {"listen", 5},
    {"server_name", 6},
    {"root", 7},
    {"error_page", 8},
    {"client_max_body_size", 9},
    {"fastcgi_pass", 10},
    {"fastcgi_index", 11},
    {"fastcgi_param", 12},
    {"index", 13},
    {"autoindex", 14},
    {"log_format", 15},
    {"events", 16},
    {"http", 17},
    {"server", 18},
    {"location", 19},
    {"limit_except", 20},
    // {"allow", 21},
    {"deny", 21},
    {"include", 22},
    {"return", 23}
};

map<string, vector<Syntax> > Parser::_directiveSyntax = {
    {"error_log", {{{Tag::ID}, 1}, {{Tag::ID}, 0}}},
    {"worker_connections", {{{Tag::NUM}, 1}}},
    {"default_type", {{{Tag::ID}, 1}}},
    {"keepalive_timeout", {{{Tag::TIME}, 1}, {{Tag::TIME}, 0}}},
    {"listen", {{{Tag::NUM, Tag::IPV4}, 1}}},
    {"server_name", {{{Tag::ID}, 1}}},
    {"root", {{{Tag::ID}, 1}}},
    {"error_page", {{{Tag::HTTPCODE}, 1}, {{Tag::HTTPCODE}, 2}, {{Tag::ID}, 0}}},
    {"client_max_body_size", {{{Tag::BYTES}, 1}}},
    {"fastcgi_pass", {{{Tag::ID}, 1}}},
    {"fastcgi_index", {{{Tag::ID}, 1}}},
    {"fastcgi_param", {{{Tag::ID}, 1}, {{Tag::ID}, 1}}},
    {"index", {{{Tag::ID}, 1}, {{Tag::ID}, 2}}},
    {"autoindex", {{{Tag::ID}, 1}}},
    {"log_format", {{{Tag::ID}, 1}, {{Tag::ID}, 0}, {{Tag::ID}, 1}, {{Tag::ID}, 2}}},
    {"events", {}},
    {"http", {}},
    {"server", {}},
    {"location", {{{'=', '~', Tag::SYMBOL}, 0}, {{Tag::ID}, 1}}},
    {"limit_except", {{{Tag::METHOD}, 1}, {{Tag::METHOD}, 2}}},
    // {"allow", {{{Tag::ID, Tag::IPV4}, 1}}},
    {"deny", {{{Tag::ID}, 1}}},
    {"include", {{{Tag::ID}, 1}}},
    {"return", {{{Tag::HTTPCODE}, 1}, {{Tag::ID}, 0}}}
};