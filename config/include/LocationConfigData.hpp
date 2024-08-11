/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfigData.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 20:28:36 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/09 20:37:37 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __LOCATION_CONFIG_DATA_HPP__
# define __LOCATION_CONFIG_DATA_HPP__

#include <map>
#include <string>
#include <vector>

using namespace std;

class LocationConfigData {
private:
    string              _errorLog;
    vector<string>      _allowedMethods;
    string              _defaultType;
    ssize_t             _keepaliveTimeout; // in seconds
    ssize_t             _headerTimeout; // in seconds
    string              _root;
    map<int, string>    _errorPage;
    ssize_t             _clientMaxBodySize; // in bytes
    string              _fastcgiPass;
    string              _fastcgiIndex;
    string              _index;
    map<string, string> _fastcgiParam;
    bool                _autoindex;
    string              _accessLog;
    pair<int, string>   _return; // used to redirect
public:
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
};

#endif