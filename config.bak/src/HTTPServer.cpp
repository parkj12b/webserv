/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 20:17:13 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/10 15:19:41 by minsepar         ###   ########.fr       */
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

vector<ServerConfigData> &HTTPServer::getServerConfigData()
{
    return _serverConfigData;
}