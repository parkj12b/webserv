#include <iostream>
#include <fstream>
#include <cwchar>

#include "Lexer.hpp"
#include "Env.hpp"

using namespace std;

void check()
{
    system("leaks a.out");
}

int main()
{
    // atexit(check);
    Lexer lex("test.txt");
    // std::cin.rdbuf(file.rdbuf());
    // char c = cin.get();
    // cout << "test: " << c << endl;
    while (true)
    {
        Token *t = lex.scan();
        if (t->tag == -1)
            break;
        cout << t->toString() << endl;
        if (dynamic_cast<Word *>(t))
            cout << "lexeme: " << dynamic_cast<Word *>(t)->lexeme << endl;
        else if (dynamic_cast<Num *>(t))
            cout << "value: " << dynamic_cast<Num *>(t)->value << endl;
        else if (dynamic_cast<Real *>(t))
            cout << "value: " << dynamic_cast<Real *>(t)->value << endl;
        cout << "tag: " << t->tag << endl;
    }

    Env env(NULL);
    Directives::init();
    vector<string> args;

    env.put("cgi_param", args);
    
}