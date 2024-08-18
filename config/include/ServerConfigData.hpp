/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfigData.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 20:19:25 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/17 01:12:24 by minsepar         ###   ########.fr       */
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
public:
    map<string, map<int, LocationConfigData> > _locationConfigData;  //location에 따른 정보가 담긴 map??
    void                            addServerName(string serverName);
    void                            setLocationConfigData(string path, int prePost, LocationConfigData locationConfigData);
    vector<string>                  &getSuffixMatch();
    vector<string>                  &getServerName();
    vector<int>                     &getPort();
    LocationConfigData              *getLocationConfigData(string path, int prePost);
    Trie                            &getPrefixTrie();
    ServerConfigData();
};

#endif