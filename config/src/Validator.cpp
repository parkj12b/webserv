/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validator.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 16:30:28 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/11 18:56:30 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Validator.hpp"
#include "Parser.hpp"
#include "HTTPServer.hpp"
#include "Env.hpp"
#include "Num.hpp"
#include "Word.hpp"
#include "UtilTemplate.hpp"
#include "ServerConfig.hpp"

using namespace std;

HTTPServer    *Validator::validate()
{
    vector<ServerConfig *> serverConfig = _parser.getServerConfig();

    checkWorkerConnections();
    map<int, ServerConfigData *> &serverConfigData = _httpServer->getServerConfigData();
    cout << "num server: " << serverConfig.size() << endl;
    unordered_set<ServerConfigData *> &serverSet = _httpServer->getServerSet();
    for (size_t i = 0, size = serverConfig.size(); i < size; i++) {
        ServerConfigData *server = checkServer(serverConfig[i]);
        serverSet.insert(server);
        vector<int> &port = server->getPort();
        for (size_t j = 0, size = port.size(); j < size; j++) {
            serverConfigData.insert(make_pair(port[j], server));
        }
    }

    return _httpServer;
}

void    Validator::checkWorkerConnections()
{
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

ServerConfigData    *Validator::checkServer(ServerConfig *serverConfig)
{
    ServerConfigData *serverData = new ServerConfigData();
    
    checkServerName(serverData, serverConfig);
    checkPort(serverData, serverConfig);
    map<string, LocationConfig> &location = serverConfig->location;
    map<string, LocationConfigData> &locationData = serverData->getLocationConfigData();
    
    for (map<string, LocationConfig>::iterator it = location.begin(); it != location.end(); it++)
    {
        string key = it->first;
        LocationConfig &locationConfig = it->second;

        locationData.insert(pair<string, LocationConfigData>(key, checkLocation(locationConfig)));
    }
    return serverData;
}
void    Validator::checkServerName(ServerConfigData *serverData, ServerConfig *serverConfig)
{
    vector<vector<vector< Token *> > > *v = serverConfig->getConfig("server_name");

    if (v == NULL) {
        serverData->setServerName(DEFAULT_SERVER_NAME);
        return ;
    }
    if (v->size() != 1) {
        throw ValidatorException("server_name directive must be set only once");
    }
    string serverName = (dynamic_cast<Word *>((*v)[0][0][0]))->lexeme;
    if (serverName == "") {
        serverData->setServerName(DEFAULT_SERVER_NAME);
    }
    serverData->setServerName(serverName);
}

void    Validator::checkPort(ServerConfigData *serverData, ServerConfig *serverConfig)
{
    vector<vector<vector< Token *> > > *v = serverConfig->getConfig("listen");
    unordered_set<int> &ports = getPorts();
    vector<int> &_port = serverData->getPort();

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

LocationConfigData    Validator::checkLocation(LocationConfig &locationConfig)
{
    LocationConfigData locationData;

    checkErrorLog(locationData, locationConfig);
    checkAllowedMethod(locationData, locationConfig);
    checkDefaultType(locationData, locationConfig);
    checkKeepaliveTimeout(locationData, locationConfig);
    checkErrorLog(locationData, locationConfig);
    checkRoot(locationData, locationConfig);
    checkErrorPage(locationData, locationConfig);
    checkClientMaxBodySize(locationData, locationConfig);
    checkFastcgiPass(locationData, locationConfig);
    checkFastcgiIndex(locationData, locationConfig);
    checkFastcgiParam(locationData, locationConfig);
    checkAutoIndex(locationData, locationConfig);
    checkAccessLog(locationData, locationConfig);
    checkReturn(locationData, locationConfig);
    checkIndex(locationData, locationConfig);
    return locationData;
}

void    Validator::checkErrorLog(LocationConfigData &locationData, LocationConfig &locationConfig)
{
    vector<vector<vector< Token *> > > *v = locationConfig.getConfig("error_log");

    if (v == NULL) {
        locationData.setErrorLog(DEFAULT_ERROR_LOG);
        return ;
    }
    if (v->size() != 1) {
        throw ValidatorException("error_log directive must be set only once");
    }
    string errorLog = (dynamic_cast<Word *>((*v)[0][0][0]))->lexeme;
    if (errorLog == "") {
        locationData.setErrorLog(DEFAULT_ERROR_LOG);
    }
    locationData.setErrorLog(errorLog);
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
        return ;
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
    else
        locationData.setDefaultType(defaultType);
}

void    Validator::checkKeepaliveTimeout(LocationConfigData &locationData, LocationConfig &locationConfig)
{
    vector<vector<vector< Token *> > > *v = locationConfig.getConfig("keepalive_timeout");

    if (v == NULL) {
        locationData.setKeepaliveTimeout(DEFAULT_KEEPALIVE_TIMEOUT);
        return ;
    }
    if (v->size() != 1) {
        throw ValidatorException("keepalive_timeout directive must be set only once");
    }
    Num *num;
    if ((num = dynamic_cast<Num *>((*v)[0][0][0])))
        locationData.setKeepaliveTimeout(num->value);
    else
    {
        string keepaliveTimeout = (dynamic_cast<Word *>((*v)[0][0][0]))->lexeme;
        ssize_t keepaliveTimeoutInt = timeToSeconds(keepaliveTimeout);        
        locationData.setKeepaliveTimeout(keepaliveTimeoutInt);
    }

    if ((*v)[0].size() == 2)
    {
        Num *num;
        if ((num = dynamic_cast<Num *>((*v)[0][1][0])))
            locationData.setHeaderTimeout(num->value);
        else
        {
            string keepaliveTimeoutHeader = (dynamic_cast<Word *>((*v)[0][1][0]))->lexeme;
            ssize_t keepaliveTimeoutInt = timeToSeconds(keepaliveTimeoutHeader);
            locationData.setHeaderTimeout(keepaliveTimeoutInt);
        }
    }
    else
        locationData.setHeaderTimeout(0);
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

    map<int, string> &errorPage = locationData.getErrorPage();
    if (v == NULL) { return ; }
    for (size_t i = 0, size = (*v).size(); i < size; i++)
    {
        string errorURI = (dynamic_cast<Word *>((*v)[i][2][0]))->lexeme;
        string errorNum = (dynamic_cast<Word *>((*v)[i][0][0]))->lexeme;
        errorPage.insert(pair<int, string>(strtol(errorNum.c_str(), NULL, 10), errorURI));
        for (size_t j = 0, numArg = (*v)[i][1].size(); j < numArg; j++)
        {
            string errorNum = (dynamic_cast<Word *>((*v)[i][1][j]))->lexeme;
            errorPage.insert(pair<int, string>(strtol(errorNum.c_str(), NULL, 10), errorURI));
        }
    }
}

void    Validator::checkClientMaxBodySize(LocationConfigData &locationData, LocationConfig &locationConfig)
{
    vector<vector<vector< Token *> > > *v = locationConfig.getConfig("client_max_body_size");

    if (v == NULL) {
        locationData.setClientMaxBodySize(DEFAULT_CLIENT_MAX_BODY_SIZE);
        return ;
    }
    if (v->size() != 1) {
        throw ValidatorException("client_max_body_size directive must be set only once");
    }
    string clientMaxBodySize = (dynamic_cast<Word *>((*v)[0][0][0]))->lexeme;
    if (clientMaxBodySize == "") {
        locationData.setClientMaxBodySize(DEFAULT_CLIENT_MAX_BODY_SIZE);
        return ;
    }
    ssize_t clientMaxBodySizeInt = strtol(clientMaxBodySize.c_str(), NULL, 10);
    if (clientMaxBodySize[clientMaxBodySize.size() - 1] == 'k'
        || clientMaxBodySize[clientMaxBodySize.size() - 1] == 'K') {
        clientMaxBodySizeInt *= 1024;
    } else if (clientMaxBodySize[clientMaxBodySize.size() - 1] == 'm'
        || clientMaxBodySize[clientMaxBodySize.size() - 1] == 'M') {
        clientMaxBodySizeInt *= 1024 * 1024;
    } else if (clientMaxBodySize[clientMaxBodySize.size() - 1] == 'g'
        || clientMaxBodySize[clientMaxBodySize.size() - 1] == 'G') {
        clientMaxBodySizeInt *= 1024 * 1024 * 1024;
    }
    locationData.setClientMaxBodySize(clientMaxBodySizeInt);
}

void    Validator::checkFastcgiPass(LocationConfigData &locationData, LocationConfig &locationConfig)
{
    vector<vector<vector< Token *> > > *v = locationConfig.getConfig("fastcgi_pass");
    if (v == NULL) {
        return ;
    }
    if (v->size() != 1) {
        throw ValidatorException("fastcgi_pass directive must be set only once");
    }
    string fastcgiPass = (dynamic_cast<Word *>((*v)[0][0][0]))->lexeme;
    if (fastcgiPass == "") {
        throw ValidatorException("fastcgi_pass directive must be set");
    }
    locationData.setFastcgiPass(fastcgiPass);
}

void    Validator::checkFastcgiIndex(LocationConfigData &locationData, LocationConfig &locationConfig)
{
    vector<vector<vector< Token *> > > *v = locationConfig.getConfig("fastcgi_index");

    if (v == NULL) {
        return ;
    }
    if (v->size() != 1) {
        throw ValidatorException("fastcgi_index directive must be set only once");
    }
    string fastcgiIndex = (dynamic_cast<Word *>((*v)[0][0][0]))->lexeme;
    if (fastcgiIndex == "") {
        return ;
    }
    locationData.setFastcgiIndex(fastcgiIndex);
}

void    Validator::checkFastcgiParam(LocationConfigData &locationData, LocationConfig &locationConfig)
{
    vector<vector<vector< Token *> > > *v = locationConfig.getConfig("fastcgi_param");

    if (v == NULL) {
        return ;
    }
    map<string, string> &fastcgiParam = locationData.getFastcgiParam();
    for (size_t i = 0, size = v->size(); i < size; i++)
    {
        string key = (dynamic_cast<Word *>((*v)[i][0][0]))->lexeme;
        string value = (dynamic_cast<Word *>((*v)[i][1][0]))->lexeme;
        fastcgiParam[key] = value;
    }
}

void    Validator::checkAutoIndex(LocationConfigData &locationData, LocationConfig &locationConfig)
{
    vector<vector<vector< Token *> > > *v = locationConfig.getConfig("autoindex");

    if (v == NULL) {
        locationData.setAutoIndex(DEFAULT_AUTO_INDEX);
        return ;
    }
    if (v->size() != 1) {
        throw ValidatorException("index directive must be set only once");
    }
    string index = (dynamic_cast<Word *>((*v)[0][0][0]))->lexeme;
    if (index == "off") {
        locationData.setAutoIndex(false);
    } else if (index == "on") {
        locationData.setAutoIndex(true);
    } else {
        throw ValidatorException("invalid autoindex value");
    }
}

void    Validator::checkAccessLog(LocationConfigData &locationData, LocationConfig &locationConfig)
{
    vector<vector<vector< Token *> > > *v = locationConfig.getConfig("access_log");

    if (v == NULL) {
        locationData.setAccessLog(DEFAULT_ACCESS_LOG);
        return ;
    }
    if (v->size() != 1) {
        throw ValidatorException("access_log directive must be set only once");
    }
    string accessLog = (dynamic_cast<Word *>((*v)[0][0][0]))->lexeme;
    if (accessLog == "") {
        locationData.setAccessLog(DEFAULT_ACCESS_LOG);
    }
    else
        locationData.setAccessLog(accessLog);
}

void    Validator::checkReturn(LocationConfigData &locationData, LocationConfig &locationConfig)
{
    vector<vector<vector< Token *> > > *v = locationConfig.getConfig("return");

    if (v == NULL) {
        return ;
    }
    if (v->size() != 1) {
        throw ValidatorException("return directive must be set only once");
    }
    string  returnCode = (dynamic_cast<Word *>((*v)[0][0][0]))->lexeme;
    int     returnCodeInt = strtol(returnCode.c_str(), NULL, 10);
    if (returnCode == "") {
        throw ValidatorException("invalid return code");
    }
    string returnURL = (dynamic_cast<Word *>((*v)[0][1][0]))->lexeme;
    if ((*v)[0].size() == 2)
    {
        locationData.setReturn(pair<int, string>(returnCodeInt, returnURL));
    }
}

void    Validator::checkIndex(LocationConfigData &locationData, LocationConfig &locationConfig)
{
    vector<vector<vector< Token *> > > *v = locationConfig.getConfig("index");

    if (v == NULL) {
        locationData.setIndex(DEFAULT_INDEX);
        return ;
    }
    if (v->size() != 1) {
        throw ValidatorException("index directive must be set only once");
    }
    string index = (dynamic_cast<Word *>((*v)[0][0][0]))->lexeme;
    if (index == "") {
        locationData.setIndex(DEFAULT_INDEX);
    }
    else
        locationData.setIndex(index);
}

Validator::ValidatorException::ValidatorException(string error)
{
    err = "Error: " + error + "\n";
}

Validator::ValidatorException::~ValidatorException() throw() {}

const char *Validator::ValidatorException::what() const throw()
{
    return err.c_str();
}

Validator::Validator(Parser &parser)
    : _parser(parser), _httpServer(new HTTPServer()) {}

Validator::~Validator()
{
    delete _httpServer;
}