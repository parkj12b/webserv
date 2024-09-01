/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfigData.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 20:19:25 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/30 19:08:09 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_CONFIG_DATA_HPP__
# define __SERVER_CONFIG_DATA_HPP__

#include <string>
#include <vector>
#include <map>
#include "LocationConfigData.hpp"
#include "Trie.hpp"

using namespace std;

/**
 * @brief                       class used for storing server config data.
 * @param   _locationConfigData location config data in current server
 */
class ServerConfigData { 
private:
    vector<string>                  _serverName;
    vector<int>                     _port;
    Trie                            _prefixTrie;
    vector<string>                  _suffixMatch;
    ssize_t                         _keepaliveTimeout;
    ssize_t                         _headerTimeout;
    map<int, string>                _errorPage;
public:
    map<string, map<int, LocationConfigData> > _locationConfigData;  //location에 따른 정보가 담긴 map??
    void                            addServerName(string serverName);
    void                            setLocationConfigData(string path, int prePost, LocationConfigData locationConfigData);
    void                            setKeepaliveTimeout(ssize_t time);
    ssize_t                         getKeepaliveTimeout();
    void                            setHeaderTimeout(ssize_t time);
    ssize_t                         getHeaderTimeout();
    vector<string>                  &getSuffixMatch();
    vector<string>                  &getServerName();
    vector<int>                     &getPort();
    LocationConfigData              *getLocationConfigData(string path, int prePost);
    Trie                            &getPrefixTrie();
    map<int, string>                &getErrorPage();
    ServerConfigData();
};

#endif