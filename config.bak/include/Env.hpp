/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Env.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 14:29:44 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/10 15:11:42 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __ENV_HPP__
# define __ENV_HPP__

#include <map>
#include <unordered_map>
#include <set>
#include <string>
#include <vector>

using namespace std;

class Token;

class Env {
private:
    string _context;
    vector<vector< Token *> > _headDirective;
    //"key", [num of occurence][argument number][multiple arguments]
    map<string, vector< vector< vector< Token *> > > > _table;
protected:
    Env *prev;
public:
    string                      getContext();
    vector<vector< Token *> >   &getHeadDirective();
    Env                         *getPrev();
    vector< Token *>            &getHeadDirectiveByIndex(size_t index);
    void                        put(string key, vector<vector< Token *> > &args);
    vector<vector<vector< Token *> > > *get(string key);
    Env(Env *n, string context);
};


#endif