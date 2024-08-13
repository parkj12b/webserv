/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfigData.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 20:19:25 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/13 18:01:21 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_CONFIG_DATA_HPP__
# define __SERVER_CONFIG_DATA_HPP__

#include <string>
#include <vector>
#include <map>
#include "LocationConfigData.hpp"

using namespace std;

/**
 * @brief                       class used for storing server config data.
 * @param   _locationConfigData location config data in current server
 */
class ServerConfigData { 
private:
    string                          _serverName;
    vector<int>                     _port;
    map<string, LocationConfigData> _locationConfigData;  //location에 따른 정보가 담긴 map??
public:
    void                            setServerName(string serverName);
    string                          getServerName();
    vector<int>                     &getPort();
    map<string, LocationConfigData> &getLocationConfigData();
    ServerConfigData();
};

#endif