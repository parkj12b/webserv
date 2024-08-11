/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 20:17:13 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/11 18:48:15 by minsepar         ###   ########.fr       */
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

map<int, ServerConfigData *>    &HTTPServer::getServerConfigData()
{
    return _serverConfigData;
}

unordered_set<ServerConfigData *>   &HTTPServer::getServerSet()
{
    return _serverSet;
}

HTTPServer::~HTTPServer()
{
    unordered_set<ServerConfigData *>::iterator it2 = _serverSet.begin();
    while (it2 != _serverSet.end())
    {
        delete *it2;
        it2++;
    }
    _serverSet.clear();
}