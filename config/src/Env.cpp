/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Env.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 17:32:33 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/10 15:19:19 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <map>
#include "Env.hpp"

using namespace std;

Env::Env(Env *n, string context) : _context(context), prev(n) {}

string Env::getContext() { return _context; }

vector<vector< Token *> > &Env::getHeadDirective() { return _headDirective; }

Env *Env::getPrev() { return prev; }

vector< Token *> &Env::getHeadDirectiveByIndex(size_t index)
{
    while (_headDirective.size() <= index)
    {
        _headDirective.push_back(vector< Token *>());
    }
    return _headDirective[index];
}

void    Env::put(string key, vector<vector< Token *> > &args)
{
    map<string, vector<vector<vector< Token *> > > >::iterator v;

    v = _table.find(key);
    if (v == _table.end()) {
        vector<vector<vector< Token *> > > vec;
        vec.push_back(args);
        _table.insert(make_pair(key, vec));
    } else {
        v->second.push_back(args);
    }
}

vector<vector<vector< Token *> > > *Env::get(string key)
{
    map<string, vector<vector<vector< Token *> > > >::iterator v;

    v = _table.find(key);
    if (v == _table.end()) {
        return NULL;
    }
    return (&v->second);
}
