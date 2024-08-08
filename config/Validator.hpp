/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validator.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 16:26:25 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/08 19:16:41 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __VALIDATOR_HPP__
# define __VALIDATOR_HPP__

#include "Parser.hpp"
#include "HTTPServer.hpp"

class Validator {
private:
    Parser &_parser;
    HTTPServer *_httpServer;
public:
    HTTPServer          *validate();
    void                checkWorkerConnections();
    ServerConfigData    checkServer(ServerConfig *serverConfig);
    void                checkServerName(ServerConfigData &serverData,
                            ServerConfig *serverConfig);
    
    
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