/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 14:35:58 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/08 23:16:16 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_HPP__
# define __SERVER_HPP__

//in which form should i return the server data?

using namespace std;

#include <map>
#include <string>
#include <vector>

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
    map<int, string>    _return; // used to redirect
public:
    void setErrorLog(string errorLog) {
        _errorLog = errorLog;
    }
    string getErrorLog() {
        return _errorLog;
    }
    vector<string>  &getAllowedMethods() {
        return _allowedMethods;
    }
    void setDefaultType(string defaultType) {
        _defaultType = defaultType;
    }
    string getDefaultType() {
        return _defaultType;
    }
    void setKeepaliveTimeout(ssize_t keepaliveTimeout) {
        _keepaliveTimeout = keepaliveTimeout;
    }
    ssize_t getKeepaliveTimeout() {
        return _keepaliveTimeout;
    }
    void setHeaderTimeout(ssize_t headerTimeout) {
        _headerTimeout = headerTimeout;
    }
    ssize_t getHeaderTimeout() {
        return _headerTimeout;
    }
    void setRoot(string root) {
        _root = root;
    }
    string getRoot() {
        return _root;
    }
};

class ServerConfigData { 
private:
    string                          _serverName;
    vector<int>                     _port;
    string                          _index;
    map<string, LocationConfigData> _locationConfigData;
public:
    void setServerName(string serverName) {
        _serverName = serverName;
    }
    string getServerName() {
        return _serverName;
    }
    vector<int> &getPort() {
        return _port;
    }
    string getIndex() {
        return _index;
    }
    void setIndex(string index) {
        _index = index;
    }
    map<string, LocationConfigData> &getLocationConfigData() {
        return _locationConfigData;
    }
    ServerConfigData() {}
};

class HTTPServer {
private:
    int                         _workerConnections;
    vector<ServerConfigData>    _serverConfigData;
public:
    int     getWorkerConnections() {
        return _workerConnections;
    }
    void    setWorkerConnections(int workerConnections) {
        _workerConnections = workerConnections;
    }
    vector<ServerConfigData> &getServerConfigData() {
        return _serverConfigData;
    }
};

#endif