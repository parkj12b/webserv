/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directives.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 17:38:44 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/09 21:43:28 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __DIRECTIVES_HPP__
# define __DIRECTIVES_HPP__

#include <unordered_map>
#include <set>
#include <string>

using namespace std;

class Directives {
private:
    static unordered_map<string, set<string> > _directive;
    static unordered_map<string, set<string> > _context;
public:
    static void init();
    static bool containsDirective(string context, string directive);
    static bool containsContext(string outerContext, string innerContext);
    Directives();
};

unordered_map<string, set<string> > Directives::_directive = unordered_map<string, set<string> >();
unordered_map<string, set<string> > Directives::_context = unordered_map<string, set<string> >();

#endif