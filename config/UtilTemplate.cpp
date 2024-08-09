/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UtilTemplate.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 15:26:20 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/09 13:42:26 by minsepar         ###   ########.fr       */
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

std::string toString(int value) {
    char buffer[50]; // Ensure the buffer is large enough
    std::sprintf(buffer, "%d", value);
    return std::string(buffer);
}

ssize_t timeToSeconds(string time)
{
    ssize_t timeInt = 0;

    if (time.find_first_of("s") != string::npos) {
        time = time.substr(0, time.size() - 1);
        timeInt = strtol(time.c_str(), NULL, 10);
    } else if (time.find_first_of("ms") != string::npos) {
        time = time.substr(0, time.size() - 2);
        timeInt = strtol(time.c_str(), NULL, 10) / 1000;
    } else if (time.find_first_of("m") != string::npos) {
        time = time.substr(0, time.size() - 1);
        timeInt = strtol(time.c_str(), NULL, 10) * 60;
    } else if (time.find_first_of("h") != string::npos) {
        time = time.substr(0, time.size() - 1);
        timeInt = strtol(time.c_str(), NULL, 10) * 60 * 60;
    } else if (time.find_first_of("d") != string::npos) {
        time = time.substr(0, time.size() - 1);
        timeInt = strtol(time.c_str(), NULL, 10) * 60 * 60 * 24;
    }
    return timeInt;
}

