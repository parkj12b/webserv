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

# include <vector>
# include <map>
# include <stack>
# include <string>
# include <unordered_set>
# include "Syntax.hpp"
# include "Lexer.hpp"
# include "LocationConfig.hpp"
# include "Env.hpp"
# include "ServerConfig.hpp"
# include "Directives.hpp"
# include "Tag.hpp"

# define DEFAULT_CONFIG_PATH "./config/default.conf"

using namespace std;

class Token;
class Env;
class ServerConfig;

/**
 * @brief                       class holding parser data
 * @param   _lex                current lexer
 * @param   _look               current token
 * @param   _top                current Env
 * @param   _event              event Env for the entire config
 * @param   _lexStack           stack of lexer for include directive
 * @param   _envList            list of all Env for free
 * @param   _serverConfig       list of server config
 * @param   _directiveNum       map of directive name to number
 * @param   _directiveSyntax    map of directive name to syntax
*/
class Parser {
private:
    Lexer *_lex;
    Token *_look;
    Env *_top;
    Env *_event;
    stack<pair<Lexer *, Token *> > _lexStack;
    set<Env *> _envList;
    set<LocationConfig *> _locationList;
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

    ServerConfig            *curServer;
    LocationConfig          *curLocation;

    /* getter */

    Env                     *getEvent();
    int                     getDirectiveNum(string s);
    vector<ServerConfig *>  getServerConfig();
    /* recursive parser language  */

    void                    move();
    void                    recover();
    void                    program();
    void                    directives();
    void                    directive();
    void                    server();
    void                    context();
    void                    headDirective();

    void                    error(string s);

    /**
     * @brief       match token and get next token from lexer
     * @param   t   token to match            
    */
    void                    match(int t);

    /**
     * @brief       saveEnv table in stack for include directive
     * @param   env Env to save
    */
    void                    saveEnv(Env *env);
    void                    saveLocation(LocationConfig *location);
    /**
     * @brief           include directive logic
     * @param   path    path (file) to include
    */
    void                    include(string &path);
    
    /* initializer */
    static void             initializeDirectiveNum();
    static void             initializeDirectiveSyntax();
    Parser(Lexer &l, string context);
    ~Parser();
};

#endif