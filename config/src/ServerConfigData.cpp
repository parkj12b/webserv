/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfigData.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 20:21:17 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/30 19:01:25 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include "ServerConfigData.hpp"

using namespace std;

void ServerConfigData::addServerName(string serverName)
{
    _serverName.push_back(serverName);
}

vector<string> &ServerConfigData::getServerName() { return _serverName; }

vector<int> &ServerConfigData::getPort() { return _port; }

LocationConfigData  *ServerConfigData::getLocationConfigData(string path, int prePost)
{
    map<string, map<int, LocationConfigData> >::iterator it = _locationConfigData.find(path);
    
    if (it == _locationConfigData.end())
        return NULL;
    map<int, LocationConfigData>::iterator it2 = it->second.find(prePost);
    if (it2 == it->second.end())
        return NULL;
    return &it2->second;
}

void    ServerConfigData::setKeepaliveTimeout(ssize_t time)
{
    _keepaliveTimeout = time;
}

ssize_t    ServerConfigData::getKeepaliveTimeout()
{
    return (_keepaliveTimeout);
}

void    ServerConfigData::setHeaderTimeout(ssize_t time)
{
    _headerTimeout = time;
}

ssize_t    ServerConfigData::getHeaderTimeout()
{
    return (_headerTimeout);
}

void    ServerConfigData::setLocationConfigData(string path,
            int prePost, LocationConfigData locationConfigData)
{
    _locationConfigData[path][prePost] = locationConfigData;
}

vector<string> &ServerConfigData::getSuffixMatch() { return _suffixMatch; }

Trie &ServerConfigData::getPrefixTrie() { return _prefixTrie; }

map<int, string> &ServerConfigData::getErrorPage() { return _errorPage; }

ServerConfigData::ServerConfigData() {}
