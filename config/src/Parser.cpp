/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 18:48:59 by minsepar          #+#    #+#             */
/*   Updated: 2024/09/06 21:01:03 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
# include "Parser.hpp"

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
    throw runtime_error("near line " + toString(_lex->line) + ": " + s + ": " + _lex->getFileName());
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

map<string, int> Parser::_directiveNum = map<string, int>();

void Parser::initializeDirectiveNum() {
    Parser::_directiveNum.insert(std::make_pair("error_log", 1));
    Parser::_directiveNum.insert(std::make_pair("worker_connections", 2));
    Parser::_directiveNum.insert(std::make_pair("default_type", 3));
    Parser::_directiveNum.insert(std::make_pair("keepalive_timeout", 4));
    Parser::_directiveNum.insert(std::make_pair("listen", 5));
    Parser::_directiveNum.insert(std::make_pair("server_name", 6));
    Parser::_directiveNum.insert(std::make_pair("root", 7));
    Parser::_directiveNum.insert(std::make_pair("error_page", 8));
    Parser::_directiveNum.insert(std::make_pair("client_max_body_size", 9));
    Parser::_directiveNum.insert(std::make_pair("fastcgi_pass", 10));
    Parser::_directiveNum.insert(std::make_pair("fastcgi_index", 11));
    Parser::_directiveNum.insert(std::make_pair("fastcgi_param", 12));
    Parser::_directiveNum.insert(std::make_pair("index", 13));
    Parser::_directiveNum.insert(std::make_pair("autoindex", 14));
    Parser::_directiveNum.insert(std::make_pair("log_format", 15));
    Parser::_directiveNum.insert(std::make_pair("events", 16));
    Parser::_directiveNum.insert(std::make_pair("http", 17));
    Parser::_directiveNum.insert(std::make_pair("server", 18));
    Parser::_directiveNum.insert(std::make_pair("location", 19));
    Parser::_directiveNum.insert(std::make_pair("limit_except", 20));
    // Parser::_directiveNum.insert(std::make_pair("allow", 21)); // Uncomment if needed
    Parser::_directiveNum.insert(std::make_pair("deny", 21));
    Parser::_directiveNum.insert(std::make_pair("include", 22));
    Parser::_directiveNum.insert(std::make_pair("return", 23));
}

Syntax makeSyntax(const std::vector<int>& tags, int value) {
    Syntax s;
    s.tag = tags;
    s.required = value;
    return s;
}

map<string, vector<Syntax> > Parser::_directiveSyntax = map<string, vector<Syntax> >();

void Parser::initializeDirectiveSyntax() {
    std::vector<Syntax> v;

    // Initialize map
    v.clear();
    std::vector<int> tags;
    tags.push_back(Tag::ID);
    v.push_back(makeSyntax(tags, 1));
    tags.clear();
    tags.push_back(Tag::ID);
    v.push_back(makeSyntax(tags, 0));
    Parser::_directiveSyntax.insert(std::make_pair("error_log", v));

    v.clear();
    tags.clear();
    tags.push_back(Tag::NUM);
    v.push_back(makeSyntax(tags, 1));
    Parser::_directiveSyntax.insert(std::make_pair("worker_connections", v));

    v.clear();
    tags.clear();
    tags.push_back(Tag::ID);
    v.push_back(makeSyntax(tags, 1));
    Parser::_directiveSyntax.insert(std::make_pair("default_type", v));

    v.clear();
    tags.clear();
    tags.push_back(Tag::TIME);
    tags.push_back(Tag::NUM);
    v.push_back(makeSyntax(tags, 1));
    tags.clear();
    tags.push_back(Tag::TIME);
    tags.push_back(Tag::NUM);
    v.push_back(makeSyntax(tags, 0));
    Parser::_directiveSyntax.insert(std::make_pair("keepalive_timeout", v));

    v.clear();
    tags.clear();
    tags.push_back(Tag::NUM);
    tags.push_back(Tag::IPV4);
    v.push_back(makeSyntax(tags, 1));
    tags.clear();
    tags.push_back(Tag::ID);
    v.push_back(makeSyntax(tags, 0));
    Parser::_directiveSyntax.insert(std::make_pair("listen", v));

    v.clear();
    tags.clear();
    tags.push_back(Tag::ID);
    v.push_back(makeSyntax(tags, 1));
    tags.clear();
    tags.push_back(Tag::ID);
    v.push_back(makeSyntax(tags, 2));
    Parser::_directiveSyntax.insert(std::make_pair("server_name", v));

    v.clear();
    tags.clear();
    tags.push_back(Tag::ID);
    v.push_back(makeSyntax(tags, 1));
    Parser::_directiveSyntax.insert(std::make_pair("root", v));

    v.clear();
    tags.clear();
    tags.push_back(Tag::HTTPCODE);
    v.push_back(makeSyntax(tags, 1));
    tags.clear();
    tags.push_back(Tag::HTTPCODE);
    v.push_back(makeSyntax(tags, 2));
    tags.clear();
    tags.push_back(Tag::ID);
    v.push_back(makeSyntax(tags, 0));
    Parser::_directiveSyntax.insert(std::make_pair("error_page", v));

    v.clear();
    tags.clear();
    tags.push_back(Tag::BYTES);
    v.push_back(makeSyntax(tags, 1));
    Parser::_directiveSyntax.insert(std::make_pair("client_max_body_size", v));

    v.clear();
    tags.clear();
    tags.push_back(Tag::ID);
    v.push_back(makeSyntax(tags, 1));
    Parser::_directiveSyntax.insert(std::make_pair("fastcgi_pass", v));

    v.clear();
    tags.clear();
    tags.push_back(Tag::ID);
    v.push_back(makeSyntax(tags, 1));
    Parser::_directiveSyntax.insert(std::make_pair("fastcgi_index", v));

    v.clear();
    tags.clear();
    tags.push_back(Tag::ID);
    v.push_back(makeSyntax(tags, 1));
    tags.clear();
    tags.push_back(Tag::ID);
    v.push_back(makeSyntax(tags, 1));
    Parser::_directiveSyntax.insert(std::make_pair("fastcgi_param", v));

    v.clear();
    tags.clear();
    tags.push_back(Tag::ID);
    v.push_back(makeSyntax(tags, 1));
    tags.clear();
    tags.push_back(Tag::ID);
    v.push_back(makeSyntax(tags, 2));
    Parser::_directiveSyntax.insert(std::make_pair("index", v));

    v.clear();
    tags.clear();
    tags.push_back(Tag::ID);
    v.push_back(makeSyntax(tags, 1));
    Parser::_directiveSyntax.insert(std::make_pair("autoindex", v));

    v.clear();
    tags.clear();
    tags.push_back(Tag::ID);
    v.push_back(makeSyntax(tags, 1));
    tags.clear();
    tags.push_back(Tag::ID);
    v.push_back(makeSyntax(tags, 0));
    tags.clear();
    tags.push_back(Tag::ID);
    v.push_back(makeSyntax(tags, 1));
    tags.clear();
    tags.push_back(Tag::ID);
    v.push_back(makeSyntax(tags, 2));
    Parser::_directiveSyntax.insert(std::make_pair("log_format", v));

    // Empty vectors for "events", "http", and "server"
    v.clear();
    Parser::_directiveSyntax.insert(std::make_pair("events", v));
    Parser::_directiveSyntax.insert(std::make_pair("http", v));
    Parser::_directiveSyntax.insert(std::make_pair("server", v));

    v.clear();
    tags.clear();
    tags.push_back(Tag::SYMBOL);
    v.push_back(makeSyntax(tags, 0));
    tags.clear();
    tags.push_back(Tag::ID);
    v.push_back(makeSyntax(tags, 1));
    Parser::_directiveSyntax.insert(std::make_pair("location", v));

    v.clear();
    tags.clear();
    tags.push_back(Tag::METHOD);
    v.push_back(makeSyntax(tags, 1));
    tags.clear();
    tags.push_back(Tag::METHOD);
    v.push_back(makeSyntax(tags, 2));
    Parser::_directiveSyntax.insert(std::make_pair("limit_except", v));

    // Uncomment if needed
    // v.clear();
    // tags.clear();
    // tags.push_back(Tag::ID);
    // tags.push_back(IPV4);
    // v.push_back(makeSyntax(tags, 1));
    // Parser::_directiveSyntax.insert(std::make_pair("allow", v));

    v.clear();
    tags.clear();
    tags.push_back(Tag::ID);
    v.push_back(makeSyntax(tags, 1));
    Parser::_directiveSyntax.insert(std::make_pair("deny", v));

    v.clear();
    tags.clear();
    tags.push_back(Tag::ID);
    v.push_back(makeSyntax(tags, 1));
    Parser::_directiveSyntax.insert(std::make_pair("include", v));

    v.clear();
    tags.clear();
    tags.push_back(Tag::HTTPCODE);
    v.push_back(makeSyntax(tags, 1));
    tags.clear();
    tags.push_back(Tag::ID);
    v.push_back(makeSyntax(tags, 0));
    Parser::_directiveSyntax.insert(std::make_pair("return", v));
}

// map<string, vector<Syntax> > Parser::_directiveSyntax = {
//     {"error_log", {{{Tag::ID}, 1}, {{Tag::ID}, 0}}},
//     {"worker_connections", {{{Tag::NUM}, 1}}},
//     {"default_type", {{{Tag::ID}, 1}}},
//     {"keepalive_timeout", {{{Tag::TIME, Tag::NUM}, 1}, {{Tag::TIME, Tag::NUM}, 0}}},
//     {"listen", {{{Tag::NUM, Tag::IPV4}, 1}, {{Tag::ID}, 0}}},
//     {"server_name", {{{Tag::ID}, 1}, {{Tag::ID}, 2}}},
//     {"root", {{{Tag::ID}, 1}}},
//     {"error_page", {{{Tag::HTTPCODE}, 1}, {{Tag::HTTPCODE}, 2}, {{Tag::ID}, 0}}},
//     {"client_max_body_size", {{{Tag::BYTES}, 1}}},
//     {"fastcgi_pass", {{{Tag::ID}, 1}}},
//     {"fastcgi_index", {{{Tag::ID}, 1}}},
//     {"fastcgi_param", {{{Tag::ID}, 1}, {{Tag::ID}, 1}}},
//     {"index", {{{Tag::ID}, 1}, {{Tag::ID}, 2}}},
//     {"autoindex", {{{Tag::ID}, 1}}},
//     {"log_format", {{{Tag::ID}, 1}, {{Tag::ID}, 0}, {{Tag::ID}, 1}, {{Tag::ID}, 2}}},
//     {"events", {}},
//     {"http", {}},
//     {"server", {}},
//     {"location", {{{Tag::SYMBOL}, 0}, {{Tag::ID}, 1}}},
//     {"limit_except", {{{Tag::METHOD}, 1}, {{Tag::METHOD}, 2}}},
//     // {"allow", {{{Tag::ID, Tag::IPV4}, 1}}},
//     {"deny", {{{Tag::ID}, 1}}},
//     {"include", {{{Tag::ID}, 1}}},
//     {"return", {{{Tag::HTTPCODE}, 1}, {{Tag::ID}, 0}}}
// };