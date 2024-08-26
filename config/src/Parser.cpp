/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 18:48:59 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/25 19:46:09 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "Env.hpp"
#include "ServerConfig.hpp"
#include "Tag.hpp"
#include "Directives.hpp"


Env *Parser::getEvent() { return _event; }

int Parser::getDirectiveNum(string s) { return _directiveNum[s]; }

vector<ServerConfig *> Parser::getServerConfig() { return _serverConfig; }

void Parser::move() {
    delete _look;
    _look = _lex->scan();
    Word *w;
    if ((w = dynamic_cast<Word *>(_look)))
        LOG(cout << "lexeme: " << w->lexeme << endl);
    LOG(cout << "tag: " << _look->tag << endl);
}

void    Parser::recover()
{
    delete _lex;
    delete _look;
    _lex = _lexStack.top().first;
    _look = _lexStack.top().second;
    _lexStack.pop();
}

void Parser::match(int t)
{
    if (_look->tag == t)
        move();
    else
        error("syntax error");
    if (_look->tag == -1 && _lexStack.size() > 0)
        recover();
}

void Parser::error(string s)
{
    throw runtime_error("near line " + to_string(_lex->line) + ": " + s + ": " + _lex->getFileName());
}

void Parser::program() {
    directives();
    match(Tag::END);
}

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
    Word *w = dynamic_cast<Word *>(_look->clone());
    string context = w->lexeme;
    if (!Directives::containsDirective(_top->getContext(), context))
        error("invalid context");
    move();
    vector<Syntax> syntaxList = _directiveSyntax[context];
    vector<vector< Token *> > v;
    size_t i = 0;
    size_t numChecks = syntaxList.size();
    while (i < numChecks) {
        vector< Token *> subV;
        if (i < v.size())
            subV = v[i];
        if (i >= syntaxList.size())
            error("syntax error: too many arguments");
        Syntax s = syntaxList[i];
        vector<int> tag = s.tag;
        bool isMatched = false;
        for (size_t j = 0; j < tag.size(); j++)
        {
            if (_look->tag == tag[j]) {
                isMatched = true;
                while (i >= v.size())
                    v.push_back(vector<Token *>());
                v[i].push_back(_look->clone());
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
    string path;
    if (context == "include")
        path = dynamic_cast<Word *>(v[0][0])->lexeme;
    _top->putVar(context, v);
    free(w);
    match(';');
    if (context == "include")
        include(path);
}

void    Parser::include(string &path)
{
    ifstream file(path);

    if (!file.good())
        error("file not found: " + path);
    Lexer *lex = new Lexer(path);
    lex->line = 1;
    _lexStack.push(make_pair(_lex, _look->clone()));
    _lex = lex;
    move();
}

void    Parser::server()
{
    curServer = new ServerConfig(_top);
    LOG(cout << "in server" << endl);
    _serverConfig.push_back(curServer);
}

void    Parser::context()
{
    LOG(cout << "in context" << endl);
    Word *w = dynamic_cast<Word *>(_look->clone());
    LOG(cout << _top->getContext() << endl);
    if (!Directives::containsContext(_top->getContext(), w->lexeme))
        error("invalid context");
    Env *temp = _top;
    LocationConfig *tempLocation = curLocation;
    _top = new Env(_top, w->lexeme);
    saveEnv(_top);
    if (_directiveNum[w->lexeme] == SERVER)
        server();
    else if (_directiveNum[w->lexeme] == LOCATION)
        curLocation = new LocationConfig(_top);
    match(Tag::CONTEXT);
    headDirective(); match('{');
    directives();
    match('}');
    Token *t = NULL;
    string path;
    int prePostNum = PREFIX;
    vector<Token *> prePost = _top->getHeadDirectiveByIndex(0);
    if (prePost.size() > 0)
        prePostNum = POSTFIX;
    // LocationConfig *locationConfig = NULL;
    switch(_directiveNum[w->lexeme]) {
        case EVENTS:
            if (_event != NULL)
                error("multiple events");
            _event = _top;
            break;
        case LOCATION:
            t = _top->getHeadDirectiveByIndex(1)[0]->clone();
            path = dynamic_cast<Word *>(t)->lexeme;
            LOG(cout << "location: " << curLocation << endl);
            curLocation->setPath(path);
                
            if (tempLocation == NULL)
            {
                if (curServer->location.find(path) == curServer->location.end())
                {
                    LOG(cout << curServer << " : " << curLocation << endl);
                    curServer->location[path].insert(make_pair(prePostNum, curLocation));
                }
            }
            else
            {
                if (tempLocation->location.find(path) == tempLocation->location.end())
                {
                    LOG(cout << tempLocation << " : " << curLocation << endl);
                    tempLocation->location[path].insert(make_pair(prePostNum, curLocation));
                }
            }
            delete t;
            break;
        case LIMIT_EXCEPT:
            LOG(cout << "limit_except: " << curLocation << endl);
            t = curLocation->getEnv()->getHeadDirectiveByIndex(1)[0]->clone();
            path = dynamic_cast<Word *>(t)->lexeme;
            if (curLocation == NULL)
                throw runtime_error("limit_except without location");
            // locationConfig = curLocation->getLocationConfig(path, prePostNum);
            curLocation->setLimitExcept(_top);
            delete t;
            break;
    }
    delete w;
    curLocation = tempLocation;
    _top = temp;
}

void    Parser::headDirective()
{
    LOG(cout << "in headDirective" << endl);
    string context = _top->getContext();
    vector<Syntax> syntaxList = _directiveSyntax[context];
    size_t i = 0;
    size_t numChecks = syntaxList.size();
    while (i < numChecks) {
        if (i >= syntaxList.size())
            error("syntax error: too many arguments");
        Syntax s = syntaxList[i];
        vector<Token *> &v = _top->getHeadDirectiveByIndex(i);
        vector<int> tag = s.tag;
        bool isMatched = false;
        for (size_t j = 0; j < tag.size(); j++)
        {        
            if (_look->tag == tag[j]) {
                isMatched = true;
                v.push_back(_look->clone());
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

void    Parser::saveEnv(Env *env)
{
    _envList.insert(env);
}

void    Parser::saveLocation(LocationConfig *location)
{
    _locationList.insert(location);
}

Parser::Parser(Lexer &l, string context)
    : _lex(new Lexer(l)), _top(new Env(NULL, context)), _event(NULL), curLocation(NULL)
{ 
    _look = NULL;
    move();
}

Parser::~Parser()
{
    delete _top;
    _top = NULL;
    delete _look;
    _look = NULL;
    for (size_t i = 0; i < _serverConfig.size(); i++)
    {
        delete _serverConfig[i];
        _serverConfig[i] = NULL;
    }
    for (set<Env *>::iterator it = _envList.begin(); it != _envList.end(); it++)
    {
        delete *it;
    }
    for (set<LocationConfig *>::iterator it = _locationList.begin(); it != _locationList.end(); it++)
    {
        delete *it;
    }
    delete _lex;
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
    {"keepalive_timeout", {{{Tag::TIME, Tag::NUM}, 1}, {{Tag::TIME, Tag::NUM}, 0}}},
    {"listen", {{{Tag::NUM, Tag::IPV4}, 1}, {{Tag::ID}, 0}}},
    {"server_name", {{{Tag::ID}, 1}, {{Tag::ID}, 2}}},
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
    {"location", {{{Tag::SYMBOL}, 0}, {{Tag::ID}, 1}}},
    {"limit_except", {{{Tag::METHOD}, 1}, {{Tag::METHOD}, 2}}},
    // {"allow", {{{Tag::ID, Tag::IPV4}, 1}}},
    {"deny", {{{Tag::ID}, 1}}},
    {"include", {{{Tag::ID}, 1}}},
    {"return", {{{Tag::HTTPCODE}, 1}, {{Tag::ID}, 0}}}
};