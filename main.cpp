/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 13:33:06 by inghwang          #+#    #+#             */
/*   Updated: 2024/08/17 00:20:53 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Kq.hpp"
#include "HTTPServer.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Validator.hpp"
#include "Directives.hpp"
#include "Server.hpp"

int logs = open("./log", O_WRONLY | O_CREAT | O_TRUNC, 0644);

void    check()
{
    system("leaks webserv");
}

int main(int argc, char **argv)
{
    atexit(check);
    cout << "========parser========" << endl;
    Directives::init();
    string path = DEFAULT_CONFIG_PATH;

    if (argc == 2)
        path = argv[1];
    else if (argc != 1)
    {
        cerr << "Usage: ./webserv [config_file]" << endl;
        exit(1);
    }
    
    Lexer lex(path);
    Parser parser(lex, "main");

    parser.program();
    Validator v(parser);
    Server::serverConfig = v.validate();

    cout << Server::serverConfig->getDefaultServer()->_locationConfigData.size() << endl;

    map<string, map<int, LocationConfigData> >::iterator it = Server::serverConfig->getDefaultServer()->_locationConfigData.begin();

    while (it != Server::serverConfig->getDefaultServer()->_locationConfigData.end())
    {
        cout << it->first << endl;
        it++;
    }

    // cout << Server::serverConfig->getDefaultServer()->getLocationConfigData("/", 0)->getLocationConfigData(".html", 1)->getRoot() << endl;


    //fd를 닫지 않았을 가능성이 존재함
    std::cout<<"========http message========"<<std::endl;
    Kq  kq;
    std::ios::sync_with_stdio(false);
    while (1)
        kq.mainLoop();
    close(logs);
}
