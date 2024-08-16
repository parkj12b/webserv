/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 13:33:06 by inghwang          #+#    #+#             */
/*   Updated: 2024/08/12 14:00:59 by minsepar         ###   ########.fr       */
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

int main(void)
{
    std::cout<<"========parser========"<<std::endl;
    Directives::init();
    
    //try - catch문을 제대로 사용할 것
    Lexer lex("./config/test.txt");

    Parser parser(lex, "main");

    parser.program();
    Validator v(parser);
    Server::serverConfig = v.validate();
    //fd를 닫지 않았을 가능성이 존재함
    std::cout<<"========http message========"<<std::endl;
    Kq  kq;
    std::ios::sync_with_stdio(false);
    while (1)
        kq.mainLoop();
    close(logs);
}
