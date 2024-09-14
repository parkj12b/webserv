/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validator.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 16:30:28 by minsepar          #+#    #+#             */
/*   Updated: 2024/09/14 15:08:26 by minsepar         ###   ########.fr       */
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
#include "Trie.hpp"

using namespace std;

HTTPServer    *Validator::validate()
{
    vector<ServerConfig *> serverConfig = _parser.getServerConfig();

    checkWorkerConnections();
    map<int, map<string, ServerConfigData *> > &serverConfigData = _httpServer->getServerConfigData();
    LOG(cout << "num server: " << serverConfig.size() << endl);
    set<ServerConfigData *> &serverSet = _httpServer->getServerSet();
    for (size_t i = 0, size = serverConfig.size(); i < size; i++) {
        ServerConfigData *server = checkServer(serverConfig[i]);
        
        serverSet.insert(server);
        vector<int> &port = server->getPort();
        vector<string> &serverName = server->getServerName();
        for (size_t j = 0, size = port.size(); j < size; j++) {
            for (size_t k = 0, size = serverName.size(); k < size; k++) {
                if (serverConfigData[port[j]].find(serverName[k]) != serverConfigData[port[j]].end()) {
                    throw ValidatorException("duplicate server config");
                }
                serverConfigData[port[j]].insert(make_pair(serverName[k], server));
                if (_httpServer->getDefaultServer(port[j]) == NULL) {
                    _httpServer->setDefaultServer(port[j], server);
                }
            }
        }
    }

    return _httpServer;
}

void    Validator::checkWorkerConnections()
{
    Env *event = _parser.getEvent();
    if (event == NULL) {
        _httpServer->setWorkerConnections(DEFAULT_WORKER_CONNECTIONS);
        return ;
    }
    vector<vector<vector< Token *> > > *v = event->getVar("worker_connections");
    if (v == NULL) {
        _httpServer->setWorkerConnections(DEFAULT_WORKER_CONNECTIONS);
        return ;
    }
    if (v->size() != 1) {
        throw ValidatorException("worker_connections directive must be set only once");
    }
    int workerConnection = (dynamic_cast<Num *>((*v)[0][0][0]))->value;
    if (workerConnection > 1024 || workerConnection <= 0) {
        throw ValidatorException("invalid worker_connections number");
    }
    _httpServer->setWorkerConnections(workerConnection);
}

ServerConfigData    *Validator::checkServer(ServerConfig *serverConfig)
{
    ServerConfigData *serverData = new ServerConfigData();
    
    checkServerName(serverData, serverConfig);
    checkPort(serverData, serverConfig);
    checkServerKeepaliveTimeout(serverData, serverConfig);
    // checkDuplicateServerConfig(serverData);
    map<string, map<int, LocationConfig *> > &location = serverConfig->location;
    Trie &prefixTrie = serverData->getPrefixTrie();
    
    for (map<string, map<int, LocationConfig *> >::iterator it = location.begin();
            it != location.end(); it++)
    {
        string path = it->first;
        map<int, LocationConfig *> &locationConfig = it->second;

        if (locationConfig.find(0) != locationConfig.end())
        {
            serverData->setLocationConfigData(path, 0, checkLocation(locationConfig[0]));
            prefixTrie.insert(path);
        }
        if (locationConfig.find(1) != locationConfig.end())
        {
            serverData->setLocationConfigData(path, 1, checkLocation(locationConfig[1]));
            serverData->getSuffixMatch().push_back(path);
        }
    }
    return serverData;
}
void    Validator::checkServerName(ServerConfigData *serverData, ServerConfig *serverConfig)
{
    vector<vector<vector< Token *> > > *v = serverConfig->getConfig("server_name");

    if (v == NULL) {
        serverData->addServerName(DEFAULT_SERVER_NAME);
        return ;
    }
    if (v->size() != 1) {
        throw ValidatorException("server_name directive must be set only once");
    }
    string serverName = (dynamic_cast<Word *>((*v)[0][0][0]))->lexeme;
    if (serverName == "") {
        serverData->addServerName(DEFAULT_SERVER_NAME);
    }
    toLowerCase(serverName);
    serverData->addServerName(serverName);
    if ((*v)[0].size() > 1)
    {
        for (size_t i = 0, size = (*v)[0][1].size(); i < size; i++)
        {
            serverName = (dynamic_cast<Word *>((*v)[0][1][i]))->lexeme;
            toLowerCase(serverName);
            serverData->addServerName(serverName);
        }
    }
}

void    Validator::checkPort(ServerConfigData *serverData, ServerConfig *serverConfig)
{
    vector<vector<vector< Token *> > > *v = serverConfig->getConfig("listen");
    vector<int> &_port = serverData->getPort();

    if (v == NULL) {
        _port.push_back(DEFAULT_PORT);
        return ;
    }
    for (size_t i = 0, size = v->size(); i < size; i++) {
        int port = (dynamic_cast<Num *>((*v)[i][0][0]))->value;
        if (port > 65535 || port < 0) {
            throw ValidatorException("invalid port number");
        }
        _port.push_back(port);
        if ((*v)[i].size() > 1) {
            string str = (dynamic_cast<Word *>((*v)[i][1][0]))->lexeme;
            if (str == "default_server") {
                _httpServer->setDefaultServer(port, serverData);
                return ;
            }
            if (_httpServer->getDefaultServer(port) != NULL) {
                throw ValidatorException("default_server directive must be set only once");
            }
        }
    }
}

void    Validator::checkServerKeepaliveTimeout(ServerConfigData *serverData,
            ServerConfig *serverConfig)
{
    vector<vector<vector< Token *> > > *v = serverConfig->getConfig("keepalive_timeout");

    if (v == NULL) {
        serverData->setKeepaliveTimeout(DEFAULT_KEEPALIVE_TIMEOUT);
        return ;
    }
    if (v->size() != 1) {
        throw ValidatorException("keepalive_timeout directive must be set only once");
    }
    Num *num;
    if ((num = dynamic_cast<Num *>((*v)[0][0][0])))
        serverData->setKeepaliveTimeout(num->value);
    else
    {
        string keepaliveTimeout = (dynamic_cast<Word *>((*v)[0][0][0]))->lexeme;
        ssize_t keepaliveTimeoutInt = timeToSeconds(keepaliveTimeout);    
        serverData->setKeepaliveTimeout(keepaliveTimeoutInt);
    }

    if ((*v)[0].size() == 2)
    {
        Num *num;
        if ((num = dynamic_cast<Num *>((*v)[0][1][0])))
            serverData->setHeaderTimeout(num->value);
        else
        {
            string keepaliveTimeoutHeader = (dynamic_cast<Word *>((*v)[0][1][0]))->lexeme;
            ssize_t keepaliveTimeoutInt = timeToSeconds(keepaliveTimeoutHeader);
            serverData->setHeaderTimeout(keepaliveTimeoutInt);
        }
    }
    else
        serverData->setHeaderTimeout(0);

}

void    Validator::checkServerErrorPage(ServerConfigData *serverData, ServerConfig *serverConfig)
{

    Env *currentEnv = serverConfig->getEnv();
    map<int, string> &errorPage = serverData->getErrorPage();
    while (currentEnv != NULL)
    {
        vector<vector<vector< Token *> > > *v = serverConfig->getConfig("error_page");
        if (v == NULL) { currentEnv = currentEnv->getPrev(); continue; }
        for (int i = 0; i < (int)v->size(); i++)
        {
            string errorURI = (dynamic_cast<Word *>((*v)[i][2][0]))->lexeme;
            string errorNum = (dynamic_cast<Word *>((*v)[i][0][0]))->lexeme;
            errorPage.insert(pair<int, string>(strtol(errorNum.c_str(), NULL, 10), errorURI));
            for (size_t j = 0, numArg = (*v)[i][1].size(); j < numArg; j++)
            {
                string errorNum = (dynamic_cast<Word *>((*v)[i][1][j]))->lexeme;
                if (errorPage.find(strtol(errorNum.c_str(), NULL, 10)) != errorPage.end())
                    continue;
                errorPage.insert(pair<int, string>(strtol(errorNum.c_str(), NULL, 10), errorURI));
            }
        }
        currentEnv = currentEnv->getPrev();
    }
}

LocationConfigData    Validator::checkLocation(LocationConfig *locationConfig)
{
    LocationConfigData locationData;

    string path = locationConfig->getPath();
    locationData.setPath(path);
    checkErrorLog(locationData, *locationConfig);
    checkAllowedMethod(locationData, *locationConfig);
    checkDefaultType(locationData, *locationConfig);
    checkKeepaliveTimeout(locationData, *locationConfig);
    checkErrorLog(locationData, *locationConfig);
    checkRoot(locationData, *locationConfig);
    checkErrorPage(locationData, *locationConfig);
    checkClientMaxBodySize(locationData, *locationConfig);
    checkFastcgiPass(locationData, *locationConfig);
    checkFastcgiIndex(locationData, *locationConfig);
    checkFastcgiParam(locationData, *locationConfig);
    checkAutoIndex(locationData, *locationConfig);
    checkAccessLog(locationData, *locationConfig);
    checkReturn(locationData, *locationConfig);
    checkIndex(locationData, *locationConfig);
    map<string, map<int, LocationConfig *> >  &location = locationConfig->location;
    
    Trie &prefixTrie = locationData.getPrefixTrie();

    for (map<string, map<int, LocationConfig *> >::iterator it = location.begin();
            it != location.end(); it++)
    {
        string path = it->first;
        map<int, LocationConfig *> &locationMap = it->second;
        
        if (locationMap.find(0) != locationMap.end())
        {
            locationData.setLocationConfigData(path, 0, checkLocation(locationMap[0]));
            prefixTrie.insert(path);
        }
        if (locationMap.find(1) != locationMap.end())
        {
            locationData.setLocationConfigData(path, 1, checkLocation(locationMap[1]));
            locationData.getSuffixMatch().push_back(path);
        }
    }
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

    Env *currentEnv = locationConfig.getEnv();
    map<int, string> &errorPage = locationData.getErrorPage();
    while (currentEnv != NULL)
    {
        vector<vector<vector< Token *> > > *v = locationConfig.getConfig("error_page");
        if (v == NULL) { currentEnv = currentEnv->getPrev(); continue; }
        for (int i = 0; i < (int)v->size(); i++)
        {
            string errorURI = (dynamic_cast<Word *>((*v)[i][2][0]))->lexeme;
            string errorNum = (dynamic_cast<Word *>((*v)[i][0][0]))->lexeme;
            errorPage.insert(pair<int, string>(strtol(errorNum.c_str(), NULL, 10), errorURI));
            for (size_t j = 0, numArg = (*v)[i][1].size(); j < numArg; j++)
            {
                string errorNum = (dynamic_cast<Word *>((*v)[i][1][j]))->lexeme;
                if (errorPage.find(strtol(errorNum.c_str(), NULL, 10)) != errorPage.end())
                    continue;
                errorPage.insert(pair<int, string>(strtol(errorNum.c_str(), NULL, 10), errorURI));
            }
        }
        currentEnv = currentEnv->getPrev();
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

void    Validator::checkDuplicateServerConfig(ServerConfigData *serverData)
{
    set<pair<string, int> > &serverNames = getServerNames();
    vector<string> &serverName = serverData->getServerName();
    vector<int> &port = serverData->getPort();

    for (size_t i = 0, size = serverName.size(); i < size; i++)
    {
        pair<string, int> p = make_pair(serverName[i], port[0]);
        if (serverNames.find(p) != serverNames.end())
        {
            throw ValidatorException("duplicate server config");
        }
        serverNames.insert(p);
    }
}

set<pair<string, int> > &Validator::getServerNames()
{
    return serverNames;
}

Validator::Validator(Parser &parser)
    : _parser(parser), _httpServer(new HTTPServer()) {}

Validator::~Validator()
{
    delete _httpServer;
}