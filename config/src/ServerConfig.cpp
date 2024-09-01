/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 19:19:23 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/30 19:08:46 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <string>
#include "ServerConfig.hpp"

vector<vector<vector< Token *> > > *ServerConfig::getConfig(string key) {
    Env *cur = _env;

    while (cur != NULL)
    {
        if (cur->getVar(key) != NULL)
            return cur->getVar(key);
        cur = cur->getPrev();
    }
    return NULL;
}

LocationConfig *ServerConfig::getLocationConfig(string key, int prePost) {
    map<string, map<int, LocationConfig *> >::iterator it = location.find(key);

    if (it == location.end())
        return NULL;
    map<int, LocationConfig *>::iterator it2 = it->second.find(prePost);
    if (it2 == it->second.end())
        return NULL;
    return it2->second;
}

Env *ServerConfig::getEnv() {
    return _env;
}

ServerConfig::ServerConfig(Env *env) : _env(env) {}

ServerConfig::~ServerConfig()
{
    // delete _env;
    // _env = NULL;
}