/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validator.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 16:26:25 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/30 19:02:52 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __VALIDATOR_HPP__
# define __VALIDATOR_HPP__

#include <string>
#include <set>
#include <exception>
#include "ServerConfigData.hpp"
#include "LocationConfigData.hpp"

using namespace std;

class Parser;
class HTTPServer;
class ServerConfig;
class LocationConfig;

#define DEFAULT_WORKER_CONNECTIONS 1024
#define DEFAULT_PORT 80
#define DEFAULT_KEEPALIVE_TIMEOUT 75
#define DEFAULT_SERVER_NAME "localhost"
#define DEFAULT_INDEX "index.html"
#define DEFAULT_ERROR_LOG "logs/error.log"
#define DEFAULT_ERROR_PAGE "error_page"
#define DEFAULT_DEFAULT_TYPE "text/plain"
#define DEFAULT_ROOT "/var/www/html"
#define DEFAULT_CLIENT_MAX_BODY_SIZE (1 * 1024 * 1024)
#define DEFAULT_FASTCGI_INDEX "index.php"
#define DEFAULT_AUTO_INDEX false
#define DEFAULT_ACCESS_LOG "logs/access.log"

/**
 * @brief                       class used for validating server config
 * @param   _parser             parser object
 * @param   _httpServer         http server made after validation
 * @param   _ports              set of ports to check duplicates
*/
class Validator {
private:
    Parser      &_parser;
    HTTPServer  *_httpServer;
    set<pair<string, int> > serverNames;
public:
    class ValidatorException : public exception {
    private:
        string err;
    public:
        ValidatorException(string error);
        ~ValidatorException() throw();
        const char *what() const throw();
    };
    HTTPServer          *validate();
    void                checkWorkerConnections();
    ServerConfigData    *checkServer(ServerConfig *serverConfig);
    void                checkServerName(ServerConfigData *serverData,
                            ServerConfig *serverConfig);
    void                checkPort(ServerConfigData *serverData,
                            ServerConfig *serverConfig);
    void                checkServerKeepaliveTimeout(ServerConfigData *serverData,
                            ServerConfig *serverConfig);
    void                checkServerErrorPage(ServerConfigData *serverData,
                            ServerConfig *serverConfig);
    LocationConfigData  checkLocation(LocationConfig *locationConfig);
    void                checkErrorLog(LocationConfigData &locationData,
                            LocationConfig &locationConfig);
    void                checkAllowedMethod(LocationConfigData &locationData,
                            LocationConfig &locationConfig);
    void                checkDefaultType(LocationConfigData &locationData,
                            LocationConfig &locationConfig);
    void                checkKeepaliveTimeout(LocationConfigData &locationData,
                            LocationConfig &locationConfig);
    void                checkRoot(LocationConfigData &locationData,
                            LocationConfig &locationConfig);
    void                checkErrorPage(LocationConfigData &locationData,
                            LocationConfig &locationConfig);
    void                checkClientMaxBodySize(LocationConfigData &locationData,
                            LocationConfig &locationConfig);
    void                checkFastcgiPass(LocationConfigData &locationData,
                            LocationConfig &locationConfig);
    void                checkFastcgiParam(LocationConfigData &locationData,
                            LocationConfig &locationConfig);
    void                checkFastcgiIndex(LocationConfigData &locationData,
                            LocationConfig &locationConfig);
    void                checkAutoIndex(LocationConfigData &locationData,
                            LocationConfig &locationConfig);
    void                checkAccessLog(LocationConfigData &locationData,
                            LocationConfig &locationConfig);
    void                checkReturn(LocationConfigData &locationData,
                            LocationConfig &locationConfig);
    void                checkIndex(LocationConfigData &locationData,
                            LocationConfig &locationConfig);
    void                checkDuplicateServerConfig(ServerConfigData *serverData);
    set<pair<string, int> >         &getServerNames();
     
    Validator(Parser &parser);
    ~Validator();
};


#endif