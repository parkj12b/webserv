/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 20:08:10 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/30 19:08:32 by minsepar         ###   ########.fr       */
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

#define PREFIX 0
#define POSTFIX 1

/**
 * @brief                       server class used in parser
 * @param   location            path to location config
 * @param   _env                server env
*/
class ServerConfig {
private:
    Env *_env;
public:
    map<string, map<int,  LocationConfig *> >         location;
    vector<vector<vector< Token *> > >  *getConfig(string key);
    LocationConfig                      *getLocationConfig(string key, int prePost);
    Env                                 *getEnv();
    ServerConfig(Env *env);
    ~ServerConfig();
};

#endif