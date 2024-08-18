/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 19:22:56 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/18 20:34:18 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationConfig.hpp"
#include "Env.hpp"

Env *LocationConfig::getEnv() { return _env; }

Env *LocationConfig::getLimitExcept() { return _limitExcept; }

void LocationConfig::setLimitExcept(Env *limitExcept) { _limitExcept = limitExcept; }

vector<vector<vector< Token *> > > *LocationConfig::getConfig(string key) {
        Env *cur = _env;

        while (cur != NULL)
        {
            if (cur->getVar(key) != NULL)
                return cur->getVar(key);
            cur = cur->getPrev();
        }
        return NULL;
    }

LocationConfig *LocationConfig::getLocationConfig(string key, int prePost) {
    map<string, map<int, LocationConfig *> >::iterator it = location.find(key);

    if (it == location.end())
        return NULL;
    map<int, LocationConfig *>::iterator it2 = it->second.find(prePost);
    if (it2 == it->second.end())
        return NULL;
    return it2->second;
}

void    LocationConfig::setPath(string path) { _path = path; }

string  LocationConfig::getPath() { return _path; }

LocationConfig::LocationConfig() : _env(NULL), _limitExcept(NULL) {}

LocationConfig::LocationConfig(Env *env) : _env(env), _limitExcept(NULL), location() {}

LocationConfig::LocationConfig(Env *env, Env *limitExcept) : _env(env), _limitExcept(limitExcept) {}

LocationConfig::~LocationConfig()
{
    // delete _env;
    // _env = NULL;
    // delete _limitExcept;
    // _limitExcept = NULL;
}