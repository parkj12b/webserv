/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 14:35:58 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/10 15:12:09 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_HPP__
# define __SERVER_HPP__

#include <map>
#include <string>
#include <vector>
#include "ServerConfigData.hpp"

using namespace std;

class HTTPServer {
private:
    int                         _workerConnections;
    vector<ServerConfigData>    _serverConfigData;
public:
    int                         getWorkerConnections();
    void                        setWorkerConnections(int workerConnections);
    vector<ServerConfigData>    &getServerConfigData();
};

#endif