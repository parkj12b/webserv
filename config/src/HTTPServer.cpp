/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 20:17:13 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/15 02:02:51 by minsepar         ###   ########.fr       */
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

map<int, map<string, ServerConfigData *> >    &HTTPServer::getServerConfigData()
{
    return _serverConfigData;
}

set<ServerConfigData *>   &HTTPServer::getServerSet()
{
    return _serverSet;
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