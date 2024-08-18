/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 19:21:41 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/16 22:12:13 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __LOCATIONCONFIG_HPP__
# define __LOCATIONCONFIG_HPP__

#include <vector>
#include <string>
#include "Env.hpp"

using namespace std;

class Token;

/**
 * @brief                       class used for parsing
 * @param   _env                current location Env
 * @param   _limitExcept        limit_except for current location
*/
class LocationConfig {
private:
    Env *_env;
    Env *_limitExcept;
public:
    map<string, map<int, LocationConfig *> >         location;
    Env *getEnv();
    Env *getLimitExcept();
    void setLimitExcept(Env *limitExcept);
    LocationConfig  *getLocationConfig(string key, int prePost);
    vector<vector<vector< Token *> > > *getConfig(string key);

    LocationConfig();
    ~LocationConfig();
    LocationConfig(Env *env);
    LocationConfig(Env *env, Env *limitExcept);
};

#endif