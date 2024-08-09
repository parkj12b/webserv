/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validator.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 16:30:28 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/09 14:04:47 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <cstdlib>
#include "Validator.hpp"
#include "Lexer.hpp"
#include "UtilTemplate.cpp"


HTTPServer    *Validator::validate()
{
    vector<ServerConfig *> serverConfig = _parser.getServerConfig();

    checkWorkerConnections();

    vector<ServerConfigData> &serverConfigData = _httpServer->getServerConfigData();
    for (size_t i = 0, size = serverConfig.size(); i < size; i++) {
        serverConfigData.push_back(checkServer(serverConfig[i]));
    }

    return _httpServer;
}

void    Validator::checkWorkerConnections()
{
    // check if worker_connections is set
    // if not, set it to 1024

    Env *event = _parser.getEvent();
    vector<vector<vector< Token *> > > *v = event->get("worker_connections");
    if (v == NULL) {
        _httpServer->setWorkerConnections(DEFAULT_WORKER_CONNECTIONS);
        return ;
    }
    if (v->size() != 1) {
        throw ValidatorException("worker_connections directive must be set only once");
    }
    int workerConnection = (dynamic_cast<Num *>((*v)[0][0][0]))->value;
    _httpServer->setWorkerConnections(workerConnection);
}

ServerConfigData    Validator::checkServer(ServerConfig *serverConfig)
{
    ServerConfigData serverData;
    
    checkServerName(serverData, serverConfig);
    checkPort(serverData, serverConfig);
    checkIndex(serverData, serverConfig);
    map<string, LocationConfig> &location = serverConfig->location;
    map<string, LocationConfigData> &locationData = serverData.getLocationConfigData();
    
    for (map<string, LocationConfig>::iterator it = location.begin(); it != location.end(); it++)
    {
        string key = it->first;
        LocationConfig &locationConfig = it->second;

        locationData.insert(pair<string, LocationConfigData>(key, checkLocation(locationConfig)));
    }
    return serverData;
}
void    Validator::checkServerName(ServerConfigData &serverData, ServerConfig *serverConfig)
{
    vector<vector<vector< Token *> > > *v = serverConfig->getConfig("server_name");

    if (v == NULL) {
        serverData.setServerName(DEFAULT_SERVER_NAME);
        return ;
    }
    if (v->size() != 1) {
        throw ValidatorException("server_name directive must be set only once");
    }
    string serverName = (dynamic_cast<Word *>((*v)[0][0][0]))->lexeme;
    if (serverName == "") {
        serverData.setServerName(DEFAULT_SERVER_NAME);
    }
}

void    Validator::checkPort(ServerConfigData &serverData, ServerConfig *serverConfig)
{
    vector<vector<vector< Token *> > > *v = serverConfig->getConfig("listen");
    unordered_set<int> &ports = getPorts();
    vector<int> _port = serverData.getPort();

    if (v == NULL) {
        if (ports.find(DEFAULT_PORT) != ports.end()) {
            throw ValidatorException("duplicate port 80");
        }
        ports.insert(DEFAULT_PORT);
        _port.push_back(DEFAULT_PORT);
        return ;
    }
    for (size_t i = 0, size = v->size(); i < size; i++) {
        int port = (dynamic_cast<Num *>((*v)[i][0][0]))->value;
        if (port > 65535 || port < 0) {
            throw ValidatorException("invalid port number");
        }
        if (ports.find(port) != ports.end()) {
            throw ValidatorException("duplicate port " + toString(port));
        }
        ports.insert(port);
        _port.push_back(port);
    }
}

void    Validator::checkIndex(ServerConfigData &serverData, ServerConfig *serverConfig)
{
    vector<vector<vector< Token *> > > *v = serverConfig->getConfig("index");

    if (v == NULL) {
        serverData.setIndex(DEFAULT_INDEX);
        return ;
    }
    if (v->size() != 1) {
        throw ValidatorException("index directive must be set only once");
    }
    string index = (dynamic_cast<Word *>((*v)[0][0][0]))->lexeme;
    if (index == "") {
        serverData.setIndex(DEFAULT_INDEX);
    }
}

LocationConfigData    Validator::checkLocation(LocationConfig &locationConfig)
{
    LocationConfigData locationData;

    checkErrorLog(locationData, locationConfig);
    checkAllowedMethod(locationData, locationConfig);
    checkDefaultType(locationData, locationConfig);
    checkKeepaliveTimeout(locationData, locationConfig);

    return locationData;
}

void    Validator::checkErrorLog(LocationConfigData &locationData, LocationConfig &locationConfig)
{
    vector<vector<vector< Token *> > > *v = locationConfig.getConfig("error_log");

    
}

void    Validator::checkAllowedMethod(LocationConfigData &locationData, LocationConfig &locationConfig)
{
    Env *limitExcept = locationConfig.getLimitExcept();
    vector<string> &allowedMethods = locationData.getAllowedMethods();

    if (limitExcept == NULL) {
        allowedMethods.push_back("GET");
        allowedMethods.push_back("HEAD");
        allowedMethods.push_back("POST");
        allowedMethods.push_back("DELETE");
    }
    
    vector<vector<Token *> > &headDirective = limitExcept->getHeadDirective();
    vector<Token *> v = headDirective[0];
    string method = dynamic_cast<Word *>(v[0])->lexeme;
    allowedMethods.push_back(method);
    
    v = headDirective[1];
    for (size_t i = 0, size = v.size(); i < size; i++) {
        method = dynamic_cast<Word *>(v[i])->lexeme;
        allowedMethods.push_back(method);
    }
}

void    Validator::checkDefaultType(LocationConfigData &locationData, LocationConfig &locationConfig)
{
    vector<vector<vector< Token *> > > *v = locationConfig.getConfig("default_type");

    if (v == NULL) {
        locationData.setDefaultType(DEFAULT_DEFAULT_TYPE);
        return ;
    }
    if (v->size() != 1) {
        throw ValidatorException("default_type directive must be set only once");
    }
    string defaultType = (dynamic_cast<Word *>((*v)[0][0][0]))->lexeme;
    if (defaultType == "") {
        locationData.setDefaultType(DEFAULT_DEFAULT_TYPE);
    }
}

void    Validator::checkKeepaliveTimeout(LocationConfigData &locationData, LocationConfig &locationConfig)
{
    vector<vector<vector< Token *> > > *v = locationConfig.getConfig("keepalive_timeout");

    if (v == NULL) {
        locationData.setKeepaliveTimeout(KEEPALIVE_TIMEOUT);
        return ;
    }
    if (v->size() != 1) {
        throw ValidatorException("keepalive_timeout directive must be set only once");
    }
    string keepaliveTimeout = (dynamic_cast<Word *>((*v)[0][0][0]))->lexeme;
    ssize_t keepaliveTimeoutInt = timeToSeconds(keepaliveTimeout);

    locationData.setKeepaliveTimeout(keepaliveTimeoutInt);
    if ((*v)[0].size() == 2)
    {
        string keepaliveTimeoutHeader = (dynamic_cast<Word *>((*v)[0][1][0]))->lexeme;
        keepaliveTimeoutInt = timeToSeconds(keepaliveTimeoutHeader);
        locationData.setHeaderTimeout(keepaliveTimeoutInt);
    }
}

void    Validator::checkRoot(LocationConfigData &locationData, LocationConfig &locationConfig)
{
    vector<vector<vector< Token *> > > *v = locationConfig.getConfig("root");

    if (v == NULL) {
        locationData.setRoot(DEFAULT_ROOT);
        return ;
    }
    if (v->size() != 1) {
        throw ValidatorException("root directive must be set only once");
    }
    string root = (dynamic_cast<Word *>((*v)[0][0][0]))->lexeme;
    if (root == "") {
        throw ValidatorException("root directive must be set");
    }
    locationData.setRoot(root);
}

void    Validator::checkErrorPage(LocationConfigData &locationData, LocationConfig &locationConfig)
{
    vector<vector<vector< Token *> > > *v = locationConfig.getConfig("error_page");

    if (v == NULL) {
        locationData.setErrorPage(DEFAULT_ERROR_PAGE);
        return ;
    }
    if (v->size() != 1) {
        throw ValidatorException("error_page directive must be set only once");
    }
    string errorPage = (dynamic_cast<Word *>((*v)[0][0][0]))->lexeme;
    if (errorPage == "") {
        locationData.setErrorPage(DEFAULT_ERROR_PAGE);
    }
}