/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: devpark <devpark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 13:33:06 by inghwang          #+#    #+#             */
/*   Updated: 2024/08/21 18:10:58 by devpark          ###   ########.fr       */
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
    auto it = Server::serverConfig->getDefaultServer(80)->_locationConfigData.begin();
    while (it != Server::serverConfig->getDefaultServer(80)->_locationConfigData.end())
    {
        cout << it->first << endl;
        it++;
    }
    //fd를 닫지 않았을 가능성이 존재함
    std::cout<<"\n\n========http message========"<<std::endl;
    // std::srand(std::time(0));  //cookie 값 정할 때에 사용할 예정
    Kq  kq;
    std::ios::sync_with_stdio(false);
    while (1)
        kq.mainLoop();
    close(logs);
}
