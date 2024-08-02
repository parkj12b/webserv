/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UtilTemplate.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 15:26:20 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/02 15:41:03 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <set>

using namespace std;

set<string> createSet(const char *str[])
{
    set<string> s;
    for (int i = 0; str[i]; i++)
        s.insert(str[i]);
    return s;
}