/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Env.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 17:32:33 by minsepar          #+#    #+#             */
/*   Updated: 2024/09/14 15:05:11 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <map>
#include "Env.hpp"
#include "Token.hpp"

using namespace std;


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

void    Env::putVar(string key, vector<vector< Token *> > &args)
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

vector<vector<vector< Token *> > > *Env::getVar(string key)
{
    map<string, vector<vector<vector< Token *> > > >::iterator v;

    v = _table.find(key);
    if (v == _table.end()) {
        return NULL;
    }
    return (&v->second);
}

Env::Env(Env *n, string context) : _context(context), prev(n) {}

Env::~Env()
{
    for (map<string, vector<vector<vector< Token *> > > >::iterator it = _table.begin(); it != _table.end(); it++)
    {
        for (vector<vector<vector< Token *> > >::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++)
        {
            for (vector<vector< Token *> >::iterator it3 = it2->begin(); it3 != it2->end(); it3++)
            {
                for (vector< Token *>::iterator it4 = it3->begin(); it4 != it3->end(); it4++)
                {
                    delete *it4;
                    *it4 = NULL;
                }
            }
        }
    }

    for (vector<vector< Token *> >::iterator it = _headDirective.begin(); it != _headDirective.end(); it++)
    {
        for (vector< Token *>::iterator it2 = it->begin(); it2 != it->end(); it2++)
        {
            delete *it2;
            *it2 = NULL;
        }
    }
}

