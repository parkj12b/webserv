/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 13:33:06 by inghwang          #+#    #+#             */
/*   Updated: 2024/08/11 21:23:45 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Kq.hpp"
#include "HTTPServer.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Validator.hpp"
#include "Directives.hpp"

int logs = open("./log", O_WRONLY | O_CREAT | O_TRUNC, 0644);

int main(void)
{
    Directives::init();
    Lexer lex("./config/test.txt");
    Parser parser(lex, "main");
    Kq  kq;

    parser.program();
    Validator v(parser);
    Kq::serverConfig = v.validate();
    //cgi실행할 fork
    std::ios::sync_with_stdio(false);
    while (1)
        kq.mainLoop();
    close(logs);
}
