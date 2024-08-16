/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 20:17:13 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/15 16:01:56 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPServer.hpp"

int HTTPServer::getWorkerConnections()
{
    return _workerConnections;
}

void HTTPServer::setWorkerConnections(int workerConnections)
{
    _workerConnections = workerConnections;
}

void    HTTPServer::setDefaultServer(ServerConfigData *server)
{
    _defaultServer = server;
}

ServerConfigData    *HTTPServer::getDefaultServer()
{
    return _defaultServer;
}

map<int, map<string, ServerConfigData *> >    &HTTPServer::getServerConfigData()
{
    return _serverConfigData;
}

ServerConfigData *HTTPServer::getServerData(string host, int port)
{
    map<int, map<string, ServerConfigData *> >::iterator it
        = _serverConfigData.find(port);
    if (it == _serverConfigData.end())
        throw runtime_error("no server data found");
    map<string, ServerConfigData *>::iterator it2
        = it->second.find(host);
    if (it2 == it->second.end())
        throw runtime_error("no server data found");
    return (it2->second);
}

set<ServerConfigData *>   &HTTPServer::getServerSet()
{
    return _serverSet;
}

HTTPServer::HTTPServer()
{
    _defaultServer = NULL;
}

HTTPServer::~HTTPServer()
{
    set<ServerConfigData *>::iterator it2 = _serverSet.begin();
    while (it2 != _serverSet.end())
    {
        delete *it2;
        it2++;
    }
    _serverSet.clear();
}