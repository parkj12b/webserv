/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 20:08:10 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/10 15:15:06 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVERCONFIG_HPP__
# define __SERVERCONFIG_HPP__

#include <vector>
#include <map>
#include "LocationConfig.hpp"

using namespace std;

class Env;
class Token;

class ServerConfig {
private:
    Env *_env;
public:
    map<string, LocationConfig> location;
    vector<vector<vector< Token *> > > *getConfig(string key);
    LocationConfig &getLocationConfig(string key);
    ServerConfig(Env *env);
};

#endif