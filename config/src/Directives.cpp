/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directives.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 17:40:11 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/13 15:33:33 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Directives.hpp" 
#include "UtilTemplate.hpp"

Directives::Directives(){}

bool    Directives::containsDirective(string context, string directive)
{
    return _directive[directive].find(context)
        != _directive[directive].end();
}

bool    Directives::containsContext(string outerContext, string innerContext)
{ 
    return _context[innerContext].find(outerContext)
        != _context[innerContext].end();
}

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

map<string, set<string> > Directives::_directive = map<string, set<string> >();
map<string, set<string> > Directives::_context = map<string, set<string> >();
