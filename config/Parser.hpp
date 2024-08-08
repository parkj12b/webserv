/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/0 23:12:20 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/06 20:36:27 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __PARSER_HPP__
# define __PARSER_HPP__

#include "Lexer.hpp"
#include "Env.hpp"
#include "ServerConfig.hpp"

class Syntax {
public:
    vector<int> tag;
    int required; // 0 = false, 1 = true, 2 = ...  
    Syntax(vector<int> t, int r) : tag(t), required(r) {}
};

class Parser {
private:
    Lexer &_lex;
    Token *_look;
    Env *_top;
    Env *_event;
    vector<ServerConfig *> _serverConfig;
    static map<string, int> _directiveNum;
    static map<string, vector<Syntax> > _directiveSyntax;
public:
    enum DirectiveNum {
        ERROR_LOG = 1,
        WORKER_CONNECTION,
        DEFAULT_TYPE,
        KEEPALIVE_TIMEOUT,
        LISTEN,
        SERVER_NAME,
        ROOT,
        ERROR_PAGE,
        CLIENT_MAX_BODY_SIZE,
        FASTCGI_PASS,
        FASTCGI_INDEX,
        FASTCGI_PARAM,
        INDEX,
        AUTOINDEX,
        LOG_FORMAT,
        EVENTS,
        HTTP,
        SERVER,
        LOCATION,
        LIMIT_EXCEPT,
        // ALLOW,
        DENY,
        INCLUDE
    };
    ServerConfig *curServer;
    Parser(Lexer &l, string context)
        : _lex(l), _top(new Env(NULL, context)), _event(NULL) { move(); }
    Env *getEvent() { return _event; }
    int getDirectiveNum(string s) { return _directiveNum[s]; }
    vector<ServerConfig *> getServerConfig() { return _serverConfig; }
    void move() {
        cout << "move called\n";
        _look = _lex.scan(); 
        cout << _look->tag << endl;
    }
    void error(string s) { throw runtime_error("near line " + to_string(_lex.line) + ": " + s); }
    void match(int t) {
        if (_look->tag == t) move();
        else error("syntax error");
    }
    void program() {
        directives();
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

    void server() {
        curServer = new ServerConfig(_top);
        cout << "in server" << endl;
        _serverConfig.push_back(curServer);
    }

    void context() {
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

    void headDirective() {
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

};

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


#endif