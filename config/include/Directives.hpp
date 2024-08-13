/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directives.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 17:38:44 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/13 15:51:14 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __DIRECTIVES_HPP__
# define __DIRECTIVES_HPP__

#include <map>
#include <set>
#include <string>

using namespace std;

/**
 * @brief               Directives -> direvice | context in parser language
 * @details             Class used to check context scope of directive / context
 * @param _directive    map of directive to context
 * @param _context      map of context to context
*/
class Directives {
private:
    static map<string, set<string> > _directive;
    static map<string, set<string> > _context;
public:
    /* init static variables _directive and _context */
    static void init();
    static bool containsDirective(string context, string directive);
    static bool containsContext(string outerContext, string innerContext);
    Directives();
};

#endif