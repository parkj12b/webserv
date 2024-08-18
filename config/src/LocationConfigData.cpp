/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfigData.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 20:29:44 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/18 20:35:45 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include <string>
#include <vector>
#include "LocationConfigData.hpp"


using namespace std;

void LocationConfigData::setPath(string path)
{
    _path = path;
}

string LocationConfigData::getPath()
{
    return _path;
}

void LocationConfigData::setErrorLog(string errorLog)
{
    _errorLog = errorLog;
}

string LocationConfigData::getErrorLog()
{
    return _errorLog;
}

vector<string> &LocationConfigData::getAllowedMethods()
{
    return _allowedMethods;
}

void LocationConfigData::setDefaultType(string defaultType)
{
    _defaultType = defaultType;
}

string LocationConfigData::getDefaultType()
{
    return _defaultType;
}

void LocationConfigData::setKeepaliveTimeout(ssize_t keepaliveTimeout)
{
    _keepaliveTimeout = keepaliveTimeout;
}

ssize_t LocationConfigData::getKeepaliveTimeout()
{
    return _keepaliveTimeout;
}

void LocationConfigData::setHeaderTimeout(ssize_t headerTimeout)
{
    _headerTimeout = headerTimeout;
}

ssize_t LocationConfigData::getHeaderTimeout()
{
    return _headerTimeout;
}

void LocationConfigData::setRoot(string root)
{
    _root = root;
}

string LocationConfigData::getRoot()
{
    return _root;
}

map<int, string> &LocationConfigData::getErrorPage()
{
    return _errorPage;
}

void LocationConfigData::setClientMaxBodySize(ssize_t clientMaxBodySize)
{
    _clientMaxBodySize = clientMaxBodySize;
}

ssize_t LocationConfigData::getClientMaxBodySize()
{
    return _clientMaxBodySize;
}

void LocationConfigData::setFastcgiPass(string fastcgiPass)
{
    _fastcgiPass = fastcgiPass;
}

string LocationConfigData::getFastcgiPass()
{
    return _fastcgiPass;
}

void LocationConfigData::setFastcgiIndex(string fastcgiIndex)
{
    _fastcgiIndex = fastcgiIndex;
}

string LocationConfigData::getFastcgiIndex()
{
    return _fastcgiIndex;
}

void LocationConfigData::setIndex(string index)
{
    _index = index;
}

string LocationConfigData::getIndex()
{
    return _index;
}

map<string, string> &LocationConfigData::getFastcgiParam()
{
    return _fastcgiParam;
}

void LocationConfigData::setAutoindex(bool autoindex)
{
    _autoindex = autoindex;
}

bool LocationConfigData::getAutoindex()
{
    return _autoindex;
}

void LocationConfigData::setAutoIndex(bool autoindex)
{
    _autoindex = autoindex;
}

string LocationConfigData::getAccessLog()
{
    return _accessLog;
}

void LocationConfigData::setAccessLog(string accessLog)
{
    _accessLog = accessLog;
}

pair<int, string> &LocationConfigData::getReturn()
{
    return _return;
}

void LocationConfigData::setReturn(pair<int, string> returnPair)
{
    _return = returnPair;
}

LocationConfigData  *LocationConfigData::getLocationConfigData(string path, int prePost)
{
    map<string, map<int, LocationConfigData> >::iterator it = _locationConfigData.find(path);
    
    if (it == _locationConfigData.end())
        return NULL;
    map<int, LocationConfigData>::iterator it2 = it->second.find(prePost);
    if (it2 == it->second.end())
        return NULL;
    return &it2->second;
}

void    LocationConfigData::setLocationConfigData(string path,
            int prePost, LocationConfigData locationConfigData)
{
    _locationConfigData[path][prePost] = locationConfigData;
}

Trie &LocationConfigData::getPrefixTrie() { return _prefixTrie; }

vector<string> &LocationConfigData::getSuffixMatch() { return _suffixMatch; }