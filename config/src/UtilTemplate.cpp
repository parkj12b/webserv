/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UtilTemplate.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 15:26:20 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/18 16:24:47 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <climits>
#include <cstdlib>
#include "UtilTemplate.hpp"


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
        case '~':
        case '*':
            return true;
    }
    return false;
}

string toString(int value) {
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

string intToUtf8(int codePoint) {
    string utf8String;

    if (codePoint < 0 || codePoint > 0x10FFFF) {
        throw out_of_range("Code point out of range for Unicode");
    }

    if (codePoint <= 0x7F) {
        // 1-byte UTF-8
        utf8String += static_cast<char>(codePoint);
    } else if (codePoint <= 0x7FF) {
        // 2-byte UTF-8
        utf8String += static_cast<char>(0xC0 | (codePoint >> 6));
        utf8String += static_cast<char>(0x80 | (codePoint & 0x3F));
    } else if (codePoint <= 0xFFFF) {
        // 3-byte UTF-8
        utf8String += static_cast<char>(0xE0 | (codePoint >> 12));
        utf8String += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
        utf8String += static_cast<char>(0x80 | (codePoint & 0x3F));
    } else {
        // 4-byte UTF-8
        utf8String += static_cast<char>(0xF0 | (codePoint >> 18));
        utf8String += static_cast<char>(0x80 | ((codePoint >> 12) & 0x3F));
        utf8String += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
        utf8String += static_cast<char>(0x80 | (codePoint & 0x3F));
    }

    return utf8String;
}

string getLineFromFile(string filename, int lineNum)
{
    ifstream file(filename);
    string line;
    for (int i = 0; i < lineNum; i++)
        getline(file, line);
    return line;
}

string getErrorAngle(int column)
{
    string angle = "";
    for (int i = 1; i < column; i++)
        angle += " ";
    angle += "^";
    return angle;
}

int isDirectory(const char *path)
{
    struct stat statbuf;

    if (stat(path, &statbuf) != 0)
        return 0;
    if (S_ISDIR(statbuf.st_mode))
        return 1;
    return 0;
}

void toLowerCase(string &str) {
    for (size_t i = 0; i < str.length(); ++i) {
        // Check if the character is uppercase
        if (str[i] >= 'A' && str[i] <= 'Z') {
            // Convert to lowercase by adding the difference between 'a' and 'A'
            str[i] = str[i] + ('a' - 'A');
        }
    }
}

bool endsWith(const std::string& text, const std::string& suffix) {
    if (suffix.length() > text.length()) {
        return false; // suffix is longer than text
    }
    return text.compare(text.length() - suffix.length(), suffix.length(), suffix) == 0;
}

bool    isWithinBasePath(const string &basePath, const string &requestPath)
{
    string resolvedPath;
    char *resolvedPathCstr;
    
    resolvedPathCstr = realpath(requestPath.c_str(), NULL);
    if (!resolvedPathCstr)
        return false;
    resolvedPath = resolvedPathCstr;
    free(resolvedPathCstr);
    if (resolvedPath.find(basePath) == 0)
        return true;
    return false;
}