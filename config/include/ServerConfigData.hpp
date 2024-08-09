/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfigData.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 20:19:25 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/09 22:09:56 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_CONFIG_DATA_HPP__
# define __SERVER_CONFIG_DATA_HPP__

#include <string>
#include <vector>
#include <map>
#include "LocationConfigData.hpp"

using namespace std;

class ServerConfigData { 
private:
    string                          _serverName;
    vector<int>                     _port;
    map<string, LocationConfigData> _locationConfigData;
public:
    void setServerName(string serverName);
    string getServerName();
    vector<int> &getPort();
    map<string, LocationConfigData> &getLocationConfigData();
    ServerConfigData();
};

#endif