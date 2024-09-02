/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 13:33:06 by inghwang          #+#    #+#             */
/*   Updated: 2024/09/01 21:31:25 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Kq.hpp"
#include "HTTPServer.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Validator.hpp"
#include "Directives.hpp"
#include "Server.hpp"
#include <sstream>
#include <streambuf>
#include <errno.h>
#include <csignal>

int logs = open("./log", O_WRONLY | O_CREAT | O_TRUNC, 0644);
int writeLogs = open("./writeLog", O_WRONLY | O_CREAT | O_TRUNC, 0644);

void    check()
{
    system("leaks webserv");
}

void signal_handler(int signal) {
    std::cout << "Caught signal " << signal << std::endl;
    // Optionally, you can handle cleanup or terminate the program
    // std::exit(0); // Uncomment to terminate the program
}

bool	findPathEnv(char **envp, string& pathEnv)
{
	int		idx = 0;
	while (envp[idx])
	{
		string oneEnv(envp[idx++]);
		size_t equalPos = oneEnv.find("=");
		if (equalPos == string::npos)
			break ;
		string envKey = oneEnv.substr(0, equalPos);
		if (!envKey.compare("PATH"))
		{
			pathEnv = oneEnv.substr(equalPos + 1);
			return (true);
		}
	}
	return (false);
}

int main(int argc, char **argv, char **envp)
{
    // atexit(check);
    cout << "========parser========" << endl;
    Directives::init();
    string path = DEFAULT_CONFIG_PATH;
    // std::signal(SIGPIPE, signal_handler);
    if (argc == 2)
        path = argv[1];
    else if (argc != 1)
    {
        cerr << "Usage: ./webserv [config_file]" << endl;
        exit(1);
    }
	string pathEnv;
	if (!findPathEnv(envp, pathEnv))
		exit(2);
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
    Kq  kq(pathEnv);
    std::ios::sync_with_stdio(false);
    while (1)
        kq.mainLoop();   
    close(logs);
    close(writeLogs);
}
