/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 20:17:13 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/17 17:00:24 by minsepar         ###   ########.fr       */
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

void    HTTPServer::setDefaultServer(int port, ServerConfigData *server)
{
    _defaultServer[port] = server;
}

ServerConfigData    *HTTPServer::getDefaultServer(int port)
{
    if (_defaultServer.find(port) == _defaultServer.end())
        return NULL;
    return _defaultServer[port];
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