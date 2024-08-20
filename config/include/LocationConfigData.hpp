/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfigData.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 20:28:36 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/18 20:35:23 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __LOCATION_CONFIG_DATA_HPP__
# define __LOCATION_CONFIG_DATA_HPP__

#include <map>
#include <string>
#include <vector>
#include "Trie.hpp"

using namespace std;

/**
 * @brief                       class used for storing location config data.
 *                              Passed to main logic
 * @param   _errorLog           error log file path
 * @param   _allowedMethods     allowed methods
 * @param   _defaultType        default type
 * @param   _keepaliveTimeout   keepalive timeout, in seconds
 * @param   _headerTimeout      header timeout, in secods
 * @param   _root               root directory
 * @param   _errorPage          error page to show in case of error
 * @param   _clientMaxBodySize  client max body size, in bytes
 * @param   _fastcgiPass        fastcgi pass, path to fastcgi
 * @param   _fastcgiIndex       fastcgi index
 * @param   _index              index
 * @param   _fastcgiParam       fastcgi param
 * @param   _autoindex          autoindex on / off
 * @param   _accessLog          access log file path
 * @param   _return             return code, return path? or message?
*/
class LocationConfigData {
private:
    string              _path;
    string              _errorLog;
    vector<string>      _allowedMethods;
    string              _defaultType;
    ssize_t             _keepaliveTimeout;
    ssize_t             _headerTimeout;
    string              _root;
    map<int, string>    _errorPage;
    ssize_t             _clientMaxBodySize;
    string              _fastcgiPass;
    string              _fastcgiIndex;
    string              _index;
    map<string, string> _fastcgiParam;
    bool                _autoindex;
    string              _accessLog;
    pair<int, string>   _return; // used to redirect
    Trie                _prefixTrie;
    vector<string>      _suffixMatch;
public:
    map<string, map<int, LocationConfigData> > _locationConfigData;  //location에 따른 정보가 담긴 map??
    void                setPath(string path);
    void                setErrorLog(string errorLog);
    void                setDefaultType(string defaultType);
    void                setKeepaliveTimeout(ssize_t keepaliveTimeout);
    void                setHeaderTimeout(ssize_t headerTimeout);
    void                setRoot(string root);
    void                setClientMaxBodySize(ssize_t clientMaxBodySize);
    void                setFastcgiPass(string fastcgiPass);
    void                setFastcgiIndex(string fastcgiIndex);
    void                setIndex(string index);
    void                setAutoindex(bool autoindex);
    void                setAutoIndex(bool autoindex);
    void                setAccessLog(string accessLog);
    void                setReturn(pair<int, string> returnPair);
    string              getPath();
    string              getErrorLog();
    vector<string>      &getAllowedMethods();
    string              getDefaultType();
    ssize_t             getKeepaliveTimeout();
    ssize_t             getHeaderTimeout();
    string              getRoot();
    map<int, string>    &getErrorPage();
    ssize_t             getClientMaxBodySize();
    string              getFastcgiPass();
    string              getFastcgiIndex();
    string              getIndex();
    map<string, string> &getFastcgiParam();
    bool                getAutoindex();
    string              getAccessLog();
    pair<int, string>   &getReturn();
    void                setLocationConfigData(string path, int prePost, LocationConfigData locationConfigData);
    LocationConfigData  *getLocationConfigData(string path, int prePost);
    Trie                &getPrefixTrie();
    vector<string>      &getSuffixMatch();
};

#endif