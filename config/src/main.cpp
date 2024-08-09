#include <iostream>
#include <fstream>
#include <cwchar>
#include <exception>

#include "Lexer.hpp"
#include "Parser.hpp"
#include "Env.hpp"
#include "Validator.hpp"
#include "Directives.hpp"

using namespace std;

void check()
{
    system("leaks a.out");
}

int main()
{
    Directives::init();
    // atexit(check);
    Lexer lex("test.txt");
    Parser parser(lex, "main");
    // std::cin.rdbuf(file.rdbuf());
    // char c = cin.get();
    // cout << "test: " << c << endl;
    
    
    // testing lexer module
    // while (true)
    // {
    //     Token *t = NULL;
    //     try {
    //         t = lex.scan();
    //     } catch (exception &e) {
    //         cout << e.what() << endl;
    //         break;
    //     }
    //     if (t->tag == -1)
    //         break;
    //     cout << t->toString() << endl;
    //     if (dynamic_cast<Word *>(t))
    //         cout << "lexeme: " << dynamic_cast<Word *>(t)->lexeme << endl;
    //     else if (dynamic_cast<Num *>(t))
    //         cout << "value: " << dynamic_cast<Num *>(t)->value << endl;
    //     cout << "tag: " << t->tag << endl;
    // }

    //testing parser module with lexer
    // try {
    //     parser.program();
    //     vector<ServerConfig *> v = parser.getServerConfig();
    //     cout << v.size() << endl;
    // } catch (exception &e) {
    //     cout << e.what() << endl;
    // }

    parser.program();
    Validator v(parser);
    v.validate();
}