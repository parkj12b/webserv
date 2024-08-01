#include <iostream>
#include <fstream>
#include <cwchar>

#include "lexer.hpp"

using namespace std;

int main()
{
    Lexer lex("test.txt");
    // std::cin.rdbuf(file.rdbuf());
    // char c = cin.get();
    // cout << "test: " << c << endl;
    while (true)
    {
        Token t = lex.scan();
        if (t.tag == -1)
            break;
        cout << t.toString() << endl;
        cout << "tag: " << t.tag << endl;
    }
}