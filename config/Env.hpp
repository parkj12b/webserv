/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Env.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 14:29:44 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/04 16:48:32 by minsepar         ###   ########.fr       */
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
    vector<string> _headDirective;
    map<string, vector<vector<string> > > _table;
protected:
    Env *prev;
public:
    Env(Env *n, string context) : _context(context), prev(n) {}
    string getContext() { return _context; }
    vector<string> &getHeadDirective() { return _headDirective; }
    void put(string key, vector<string> args) {
        map<string, vector<vector<string> > >::iterator v;

        v = _table.find(key);
        if (v == _table.end()) {
            vector<vector<string> > vec;
            vec.push_back(args);
            _table.insert(pair<string, vector<vector<string> > >(key, vec));
        } else {
            v->second.push_back(args);
        }
    }
    vector<vector<string> > *get(string key) {
        map<string, vector<vector<string> > >::iterator v;

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
    Directives() {}
};

unordered_map<string, set<string> > Directives::_directive = unordered_map<string, set<string> >();
unordered_map<string, set<string> > Directives::_context = unordered_map<string, set<string> >();

void Directives::init()
{
    const char *error_log[] = {"main", "http", "mail", "stream", "server", "location"};
    const char *worker_connection[] = {"events"};
    const char *default_type[] = {"http", "server", "location"};
    const char *keepalive_timeout[] = {"http", "server", "location"};
    const char *listen[] = {"server"};
    const char *server_name[] = {"server"};
    const char *root[] = {"http", "server", "location"};
    const char *error_page[] = {"http", "server", "location"};
    const char *client_max_body_size[] = {"http", "server", "location"};
    const char *fastcgi_pass[] = {"location"};
    const char *fastcgi_index[] = {"http", "server", "location"};
    const char *fastcgi_param[] = {"http", "server", "location"};
    const char *index[] = {"http", "server", "location"};
    const char *autoindex[] = {"http", "server", "location"};
    const char *log_format[] = {"http"};
    _directive.insert(make_pair("error_log", createSet(error_log)));
    _directive.insert(make_pair("worker_connection", createSet(worker_connection)));
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

    const char *events[] = {"main"};
    const char *http[] = {"main"};
    const char *server[] = {"http"};
    const char *location[] = {"server", "location"};
    const char *limit_except[] = {"location"};
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