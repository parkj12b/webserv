/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UtilTemplate.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 15:26:20 by minsepar          #+#    #+#             */
/*   Updated: 2024/09/07 14:32:01 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
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

string toString(size_t value) {
    stringstream ss;
	ss << value;
    return string(ss.str());
}

ssize_t timeToSeconds(string time)
{
    ssize_t timeInt = 0;

    if (time.find_first_of("s") != string::npos) {
        time = time.substr(0, time.size() - 1);
        timeInt = strtol(time.c_str(), NULL, 10);
    } else if (time.find("ms") != string::npos) {
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
    if (errno == ERANGE)
        return LONG_MAX;
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
    {
        return 0;
    }
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

bool endsWith(const string& text, const string& suffix) {
    if (suffix.length() > text.length()) {
        return false; // suffix is longer than text
    }
    return text.compare(text.length() - suffix.length(), suffix.length(), suffix) == 0;
}

bool    isWithinBasePath(const string &basePath, const string &requestPath)
{
    string resolvedPath;
    char *realBasePath;
    char *resolvedPathCstr;
    
    LOG(cout << "requestPath: " << requestPath << endl);
    realBasePath = realpath(basePath.c_str(), NULL);
    if (!realBasePath)
    {
        LOG(cout<<"403 1 1"<<endl);
        return false;
    }
    resolvedPathCstr = realpath(requestPath.c_str(), NULL);
    if (!resolvedPathCstr)
    {
        LOG(cout<<"403 1 2"<<endl);
        return false;
    }
    LOG(cout << "resolved path: " << resolvedPathCstr << endl);
    resolvedPath = resolvedPathCstr;
    delete resolvedPathCstr;
    if (resolvedPath.find(realBasePath) == 0)
    {
        delete realBasePath;
        return true;
    }
    LOG(cout<<"403 1 3"<<endl);
    return false;
}

bool    isFile(const char *path)
{
    struct stat statbuf;

    if (stat(path, &statbuf) != 0)
        return false;
    if (S_ISREG(statbuf.st_mode))
        return true;
    return false;
}

string  getDir(string path)
{
    size_t pos = path.find_last_of("/");
    if (pos == string::npos)
        return path;
    return path.substr(0, pos);
}

int throwIfError(int status, int error)
{
    if (error >= 0)
        return (1);
    LOG(cout << "errno: " << errno << endl;)
    if (status != 0)
        return (0);
    return (1);
}