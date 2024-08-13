/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 14:35:58 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/11 18:47:55 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_HPP__
# define __SERVER_HPP__

#include <map>
#include <string>
#include <vector>
#include <unordered_set>
#include "ServerConfigData.hpp"

using namespace std;

class HTTPServer
{
private:
    int                                 _workerConnections;
    map<int, ServerConfigData *>        _serverConfigData;  //포트에 알맞는 서버 config _serverSet과의 차이는??
    unordered_set<ServerConfigData *>   _serverSet;
public:
    int                                 getWorkerConnections();
    void                                setWorkerConnections(int workerConnections);
    map<int, ServerConfigData *>        &getServerConfigData();
    unordered_set<ServerConfigData *>   &getServerSet();
    ~HTTPServer();
};

#endif