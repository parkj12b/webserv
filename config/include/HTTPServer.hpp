/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 14:35:58 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/13 21:15:17 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_HPP__
# define __SERVER_HPP__

#include <map>
#include <set>
#include <string>
#include "ServerConfigData.hpp"

using namespace std;

/**
 * @brief                       server config storage
 * @param   _workerConnections  number of worker connections
 * @param   _serverConfigData   map of server config data
 * @param   _serverSet          used for freeing serverConfigData *
*/
class HTTPServer
{
private:
    int                                 _workerConnections;
    map<int, ServerConfigData *>        _serverConfigData;  //포트에 알맞는 서버 config _serverSet과의 차이는??
    set<ServerConfigData *>             _serverSet;
public:
    int                                 getWorkerConnections();
    void                                setWorkerConnections(int workerConnections);
    map<int, ServerConfigData *>        &getServerConfigData();
    set<ServerConfigData *>             &getServerSet();
    ~HTTPServer();
};

#endif