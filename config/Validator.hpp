/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validator.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 16:26:25 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/09 13:50:24 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __VALIDATOR_HPP__
# define __VALIDATOR_HPP__

#include <unordered_set>
#include "Parser.hpp"
#include "HTTPServer.hpp"

#define DEFAULT_WORKER_CONNECTIONS 1024
#define DEFAULT_PORT 80
#define KEEPALIVE_TIMEOUT 75
#define DEFAULT_SERVER_NAME "localhost"
#define DEFAULT_INDEX "index.html"
#define DEFAULT_ERROR_LOG "error.log"
#define DEFAULT_ERROR_PAGE "error_page"
#define DEFAULT_ALLOWED_METHODS "GET HEAD POST"
#define DEFAULT_DEFAULT_TYPE "text/plain"
#define DEFAULT_ROOT "/var/www/html"

class Validator {
private:
    Parser &_parser;
    HTTPServer *_httpServer;
    unordered_set<int> _ports;
public:
    HTTPServer          *validate();
    void                checkWorkerConnections();
    ServerConfigData    checkServer(ServerConfig *serverConfig);
    void                checkServerName(ServerConfigData &serverData,
                            ServerConfig *serverConfig);
    void                checkPort(ServerConfigData &serverData,
                            ServerConfig *serverConfig);
    void                checkIndex(ServerConfigData &serverData,
                            ServerConfig *serverConfig);
    LocationConfigData  checkLocation(LocationConfig &locationConfig);
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

    unordered_set<int>  &getPorts() { return _ports; }
    
    class ValidatorException : public exception {
    private:
        string err;
    public:
        ValidatorException(string error) {
            err = "Error: " + error + "\n";
        }
        ~ValidatorException() throw() {}
        const char *what() const throw() {
            return err.c_str();
        }
    };
    Validator(Parser &parser)
        : _parser(parser), _httpServer(new HTTPServer()) {}
};


#endif