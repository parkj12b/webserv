/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UtilTemplate.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 17:27:22 by minsepar          #+#    #+#             */
/*   Updated: 2024/09/05 15:37:40 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __UTILTEMPLATE_HPP__
# define __UTILTEMPLATE_HPP__


#include <set>
#include <string>

using namespace std;

#ifdef DEBUG
    #define LOG(x) x
#else
    #define LOG(x)
#endif

set<string> createSet(const char *str[]);
bool        isString(char peek);
string      toString(size_t value);
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
string      getDir(string path);

int         throwIfError(int error, int isError);
#endif