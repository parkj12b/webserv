/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Syntax.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 15:36:25 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/04 20:30:23 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SYNTAX_HPP__
# define __SYNTAX_HPP__

using namespace std;

#include <map>
#include <string>
#include <vector>

#include "Lexer.hpp"
#include "Parser.hpp"

#define ERROR -1

class Syntax {
private:
    static map<string, vector<SyntaxType> > _syntax;
public:
    static void initSyntax();
    SyntaxType getSyntax(string context, int i)
    {
        if (i < _syntax.size())
            return _syntax[context].at(i);
        else if (_syntax[context].at(_syntax.size() - 1).type[0] == Tag::VAARG)
            return _syntax[context].at(_syntax.size() - 1);
        else
            return SyntaxType{{ERROR}, false};
    }
    void Syntax::initSyntax(string fileName)
    {
        ifstream file(fileName);
    }
};

// going to use a syntax file
// type is defined by hand, isRequired is used by square brackets
// exactMatch is done by hand, using double quotes

class SyntaxType {
public:
    vector<int, string>     type;
    bool                    isRequired;
    bool                    exactMatch;
};



#endif