/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfigData.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 20:21:17 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/14 14:24:21 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfigData.hpp"

using namespace std;

void ServerConfigData::setServerName(string serverName)
{
    _serverName = serverName;
}

string ServerConfigData::getServerName() { return _serverName; }

vector<int> &ServerConfigData::getPort() { return _port; }

map<string, LocationConfigData> &ServerConfigData::getLocationConfigData()
{
    return _locationConfigData;
}

Trie &ServerConfigData::getLocationTrie() { return _locationTrie; }

ServerConfigData::ServerConfigData() {}
