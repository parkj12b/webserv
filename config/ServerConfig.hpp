/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 20:08:10 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/06 23:10:06 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVERCONFIG_HPP__
# define __SERVERCONFIG_HPP__

#include "Env.hpp"
#include "Lexer.hpp"
#include <vector>
#include <map>

class ServerConfig {
private:
    Env *_env;
public:
    vector<Env *> location;
    vector<Env *> limitExcept;
    ServerConfig(Env *env) : _env(env) {}
    vector<vector<Token *> > *getConfig(string key) {
        Env *cur = _env;

        while (cur != NULL)
        {
            if (cur->get(key) != NULL)
                return cur->get(key);
            cur = cur->getPrev();
        }
        return NULL;
    }
};

#endif