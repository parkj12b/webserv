/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UtilTemplate.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 17:27:22 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/19 13:54:56 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __UTILTEMPLATE_HPP__
# define __UTILTEMPLATE_HPP__


#include <set>
#include <string>

using namespace std;

set<string> createSet(const char *str[]);
bool        isString(char peek);
string      toString(int value);
ssize_t     timeToSeconds(string time);

/* int to utf string */
string      intToUtf8(int codePoint);

/* used in lexer */ // need to be moved to lexer ?
string      getLineFromFile(string filename, int line);
string      getErrorAngle(int column);

int         isDirectory(const char *path);
void        toLowerCase(string &str);

/* used for suffix matching */
bool        endsWith(const string& text, const string& suffix);
bool        isWithinBasePath(const string &basePath, const string &requestPath);

bool        isFile(const char *path);
#endif