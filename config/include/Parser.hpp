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

#include <vector>
#include <map>
#include <string>
#include "Lexer.hpp"

using namespace std;

class Token;
class Env;
class ServerConfig;
class Syntax;


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
    Parser(Lexer &l, string context);
    Env *getEvent();
    int getDirectiveNum(string s);
    vector<ServerConfig *> getServerConfig();
    void move();
    void error(string s) { throw runtime_error("near line " + to_string(_lex.line) + ": " + s); }
    void match(int t);
    void program();
    void directives();
    void directive();
    void server();
    void context();
    void headDirective();
};

#endif