/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 13:33:06 by inghwang          #+#    #+#             */
/*   Updated: 2024/09/14 20:44:33 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Kq.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Validator.hpp"
#include "Directives.hpp"
#include "Server.hpp"

// int readLog;

void    check()
{
    system("leaks webserv");
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
    Parser::initializeDirectiveNum();
    Parser::initializeDirectiveSyntax();
    LOG(cout << "========parser========" << endl;)
    Directives::init();
    string path = DEFAULT_CONFIG_PATH;
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

    // readLog = open("./readLog", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    //fd를 닫지 않았을 가능성이 존재함
    Kq  kq(pathEnv);
    std::ios::sync_with_stdio(false);
    while (1)
        kq.mainLoop();
    // close(readLog);
}
