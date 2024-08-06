/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Env.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 14:29:44 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/07 02:03:10 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __ENV_HPP__
# define __ENV_HPP__

#include <map>
#include <unordered_map>
#include <set>
#include <string>
#include <vector>
#include "Lexer.hpp"

set<string> createSet(const char *str[]);

class Env {
private:
    string _context;
    vector<Token *> _headDirective;
    map<string, vector<vector<Token *> > > _table;
protected:
    Env *prev;
public:
    Env(Env *n, string context) : _context(context), prev(n) {}
    string getContext() { return _context; }
    Env *getPrev() { return prev; }
    vector<Token *> &getHeadDirective() { return _headDirective; }
    void put(string key, vector<Token *> &args) {
        map<string, vector<vector<Token *> > >::iterator v;

        v = _table.find(key);
        if (v == _table.end()) {
            vector<vector<Token *> > vec;
            vec.push_back(args);
            _table.insert(make_pair(key, vec));
        } else {
            v->second.push_back(args);
        }
    }
    vector<vector<Token *> > *get(string key) {
        map<string, vector<vector<Token *> > >::iterator v;

        v = _table.find(key);
        if (v == _table.end()) {
            return NULL;
        }
        return (&v->second);
    }
};

class Directives {
private:
    static unordered_map<string, set<string> > _directive;
    static unordered_map<string, set<string> > _context;
public:
    static void init();
    static bool containsDirective(string context, string directive) {
        // cout << _directive[directive].size() << endl;
        return _directive[directive].find(context)
            != _directive[directive].end(); }
    static bool containsContext(string outerContext, string innerContext) { 
        // cout << _context[innerContext].size() << endl;
        return _context[innerContext].find(outerContext)
            != _context[innerContext].end(); }
    Directives() {}
};

unordered_map<string, set<string> > Directives::_directive = unordered_map<string, set<string> >();
unordered_map<string, set<string> > Directives::_context = unordered_map<string, set<string> >();

void Directives::init()
{
    const char *error_log[] = {"main", "http", "mail", "stream", "server", "location", NULL};
    const char *_return[] = {"server", "location", "if", NULL};
    const char *worker_connection[] = {"events", NULL};
    const char *default_type[] = {"http", "server", "location", NULL};
    const char *keepalive_timeout[] = {"http", "server", "location", NULL};
    const char *listen[] = {"server", NULL};
    const char *server_name[] = {"server", NULL};
    const char *root[] = {"http", "server", "location", NULL};
    const char *error_page[] = {"http", "server", "location", NULL};
    const char *client_max_body_size[] = {"http", "server", "location", NULL};
    const char *fastcgi_pass[] = {"location", NULL};
    const char *fastcgi_index[] = {"http", "server", "location", NULL};
    const char *fastcgi_param[] = {"http", "server", "location", NULL};
    const char *index[] = {"http", "server", "location", NULL};
    const char *autoindex[] = {"http", "server", "location", NULL};
    const char *log_format[] = {"http", NULL};
    const char *allow[] = {"http", "server", "location", "limit_except", NULL};
    const char *deny[] = {"http", "server", "location", "limit_except", NULL};
    const char *any[] = {"http", "server", "location", "limit_except", "events", "main", "mail", "stream",
        "upstream", "limit_except", "if", "map", "limit_req", "limit_conn", "split", "geo", "add_header", NULL};
    _directive.insert(make_pair("error_log", createSet(error_log)));
    _directive.insert(make_pair("worker_connections", createSet(worker_connection)));
    _directive.insert(make_pair("default_type", createSet(default_type)));
    _directive.insert(make_pair("keepalive_timeout", createSet(keepalive_timeout)));
    _directive.insert(make_pair("listen", createSet(listen)));
    _directive.insert(make_pair("server_name", createSet(server_name)));
    _directive.insert(make_pair("root", createSet(root)));
    _directive.insert(make_pair("error_page", createSet(error_page)));
    _directive.insert(make_pair("client_max_body_size", createSet(client_max_body_size)));
    _directive.insert(make_pair("fastcgi_pass", createSet(fastcgi_pass)));
    _directive.insert(make_pair("fastcgi_index", createSet(fastcgi_index)));
    _directive.insert(make_pair("fastcgi_param", createSet(fastcgi_param)));
    _directive.insert(make_pair("index", createSet(index)));
    _directive.insert(make_pair("autoindex", createSet(autoindex)));
    _directive.insert(make_pair("log_format", createSet(log_format)));
    _directive.insert(make_pair("allow", createSet(allow)));
    _directive.insert(make_pair("deny", createSet(deny)));
    _directive.insert(make_pair("include", createSet(any)));
    _directive.insert(make_pair("return", createSet(_return)));

    const char *events[] = {"main", NULL};
    const char *http[] = {"main", NULL};
    const char *server[] = {"http", NULL};
    const char *location[] = {"server", "location", NULL};
    const char *limit_except[] = {"location", NULL};
    _context.insert(make_pair("events", createSet(events)));
    _context.insert(make_pair("http", createSet(http)));
    _context.insert(make_pair("server", createSet(server)));
    _context.insert(make_pair("location", createSet(location)));
    _context.insert(make_pair("limit_except", createSet(limit_except)));
}

// unordered_map<string, set<string> > Directives::_directive = {
//     {"error_log", {"main", "http", "mail", "stream", "server", "location"}},
//     {"worker_connection", {"events"}},
//     {"default_type", {"http", "server", "location"}},
//     {"keepalive_timeout", {"http", "server", "location"}},
//     {"listen", {"server"}},
//     {"server_name", {"server"}},
//     {"root", {"http", "server", "location"}},
//     {"error_page", {"http", "server", "location"}},
//     {"client_max_body_size", {"http", "server", "location"}},
//     {"fastcgi_pass", {"location"}},
//     {"fastcgi_index", {"http", "server", "location"}},
//     {"fastcgi_param", {"http", "server", "location"}},
//     {"index", {"http", "server", "location"}},
//     {"autoindex", {"http", "server", "location"}},
//     {"log_format", {"http"}}
// };

// unordered_map<string, set<string> > Directives::_context = {
//     {"events", {"main"}},
//     {"http", {"main"}},
//     {"server", {"http"}},
//     {"location", {"server", "location"}},
//     {"limit_except", {"location"}},
// };


#endif