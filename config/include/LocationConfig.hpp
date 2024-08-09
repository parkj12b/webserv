/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 19:21:41 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/09 21:48:16 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __LOCATIONCONFIG_HPP__
# define __LOCATIONCONFIG_HPP__

#include <vector>
#include <string>

using namespace std;

class Env;
class Token;

class LocationConfig {
private:
    Env *_env;
    Env *_limitExcept;
public:
    Env *getEnv();
    Env *getLimitExcept();
    void setLimitExcept(Env *limitExcept);
    vector<vector<vector< Token *> > > *getConfig(string key);

    LocationConfig();
    LocationConfig(Env *env);
    LocationConfig(Env *env, Env *limitExcept);
};

#endif