/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UtilTemplate.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 15:26:20 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/06 23:45:36 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <set>
#include <fstream>
#include <iostream>

using namespace std;

set<string> createSet(const char *str[])
{
    set<string> s;
    for (int i = 0; str[i]; i++)
    {
        s.insert(str[i]);
    }
    return s;
}

bool isString(char peek)
{
    if (isalnum(peek))
        return true;
    switch(peek)
    {
        case '_':
        case '.':
        case '-':
        case '/':
        case ':':
        case '=':
        case '?':
        case '[':
            return true;
    }
    return false;
}
