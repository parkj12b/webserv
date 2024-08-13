/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Env.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 14:29:44 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/13 16:16:00 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __ENV_HPP__
# define __ENV_HPP__

#include <map>
#include <string>
#include <vector>

using namespace std;

class Token;

/**
 * @brief                   config variable storage
 * @param   _context        current context for the class
 * @param   _headDirective  arguments that could come in between context
 *                          and block open
 * @param   _table          table of variables
 * @param   prev            parent Env, block enclosing current context
*/
class Env {
private:
    string _context;
    vector<vector< Token *> > _headDirective;
    /* "key", [num of occurence][argument number][multiple arguments] */
    map<string, vector< vector< vector< Token *> > > > _table;
protected:
    Env *prev;
public:
    string                      getContext();
    vector<vector< Token *> >   &getHeadDirective();
    Env                         *getPrev();
    vector< Token *>            &getHeadDirectiveByIndex(size_t index);
    void                        putVar(string key, vector<vector< Token *> > &args);
    vector<vector<vector< Token *> > > *getVar(string key);
    Env(Env *n, string context);
    ~Env();
};


#endif