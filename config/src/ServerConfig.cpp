/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 19:19:23 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/13 16:17:14 by minsepar         ###   ########.fr       */
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

LocationConfig &ServerConfig::getLocationConfig(string key) {
    return location[key];
}

ServerConfig::ServerConfig(Env *env) : _env(env) {}

ServerConfig::~ServerConfig()
{
    // delete _env;
    // _env = NULL;
}