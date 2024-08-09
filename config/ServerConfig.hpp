/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 20:08:10 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/08 22:01:28 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVERCONFIG_HPP__
# define __SERVERCONFIG_HPP__

#include "Env.hpp"
#include "Lexer.hpp"
#include <vector>
#include <map>

class LocationConfig {
private:
    Env *_env;
    Env *_limitExcept;
public:
    Env *getEnv() { return _env; }
    Env *getLimitExcept() { return _limitExcept; }
    void setLimitExcept(Env *limitExcept) { _limitExcept = limitExcept; }
    vector<vector<vector< Token *> > > *getConfig(string key) {
        Env *cur = _env;

        while (cur != NULL)
        {
            if (cur->get(key) != NULL)
                return cur->get(key);
            cur = cur->getPrev();
        }
        return NULL;
    }

    LocationConfig() : _env(NULL), _limitExcept(NULL) {}
    LocationConfig(Env *env) : _env(env), _limitExcept(NULL) {}
    LocationConfig(Env *env, Env *limitExcept) : _env(env), _limitExcept(limitExcept) {}
};

class ServerConfig {
private:
    Env *_env;
public:
    map<string, LocationConfig> location;
    ServerConfig(Env *env) : _env(env) {}
    vector<vector<vector< Token *> > > *getConfig(string key) {
        Env *cur = _env;

        while (cur != NULL)
        {
            if (cur->get(key) != NULL)
                return cur->get(key);
            cur = cur->getPrev();
        }
        return NULL;
    }
    LocationConfig &getLocationConfig(string key) {
        return location[key];
    }
};

#endif