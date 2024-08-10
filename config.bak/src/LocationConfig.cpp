/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 19:22:56 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/10 16:09:58 by minsepar         ###   ########.fr       */
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
            if (cur->get(key) != NULL)
                return cur->get(key);
            cur = cur->getPrev();
        }
        return NULL;
    }

LocationConfig::LocationConfig() : _env(NULL), _limitExcept(NULL) {}

LocationConfig::LocationConfig(Env *env) : _env(env), _limitExcept(NULL) {}

LocationConfig::LocationConfig(Env *env, Env *limitExcept) : _env(env), _limitExcept(limitExcept) {}
