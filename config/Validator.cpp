/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validator.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 16:30:28 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/08 19:19:41 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Validator.hpp"
#include "Lexer.hpp"


HTTPServer    *Validator::validate()
{
    vector<ServerConfig *> serverConfig = _parser.getServerConfig();

    checkWorkerConnections();

    vector<ServerConfigData> &serverConfigData = _httpServer->getServerConfigData();
    for (size_t i = 0, size = serverConfig.size(); i < size; i++) {
        serverConfigData.push_back(checkServer(serverConfig[i]));
    }

    return _httpServer;
}

void    Validator::checkWorkerConnections()
{
    // check if worker_connections is set
    // if not, set it to 1024

    Env *event = _parser.getEvent();
    vector<vector<vector< Token *> > > *v = event->get("worker_connections");
    if (v == NULL) {
        _httpServer->setWorkerConnections(1024);
        return ;
    }
    if (v->size() != 1) {
        throw ValidatorException("worker_connections directive must be set only once");
    }
    int workerConnection = (dynamic_cast<Num *>((*v)[0][0][0]))->value;
    _httpServer->setWorkerConnections(workerConnection);
}

ServerConfigData    Validator::checkServer(ServerConfig *serverConfig)
{
    ServerConfigData serverData;
    
    checkServerName(serverData, serverConfig);
}
void    Validator::checkServerName(ServerConfigData &serverData, ServerConfig *serverConfig)
{
    vector<vector<vector< Token *> > > *v = serverConfig->getConfig("server_name");

    if (v == NULL) {
        serverData.setServerName("default_server");
        return ;
    }
    
}