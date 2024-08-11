#include <iostream>
#include <fstream>
#include <cwchar>
#include <exception>

#include "Validator.hpp"
#include "Parser.hpp"
#include "Lexer.hpp"
#include "Directives.hpp"
#include "HTTPServer.hpp"

using namespace std;

void check()
{
    system("leaks a.out");
}

void    test(HTTPServer *httpServer)
{
    cout << "worker connections: " << httpServer->getWorkerConnections() << endl;
    vector<ServerConfigData> &serverConfigData = httpServer->getServerConfigData();

    cout << "server config size: " << serverConfigData.size() << endl;
    for (size_t i = 0; i < serverConfigData.size(); i++)
    {
        ServerConfigData &serverData = serverConfigData[i];
        cout << "server name: " << serverData.getServerName() << endl;
        vector<int> &port = serverData.getPort();
        cout << "port size: " << port.size() << endl;
        for (size_t j = 0; j < port.size(); j++)
            cout << "port: " << port[j] << endl;
        map<string, LocationConfigData> &locationConfigData = serverData.getLocationConfigData();
        for (size_t j = 0; j < locationConfigData.size(); j++)
        {
            LocationConfigData locationData;
            if (j == 0)
                locationData = locationConfigData["/"];
            else if (j == 1)
                locationData = locationConfigData["/moved"];
            else if (j == 2)
                locationData = locationConfigData["/new-page"];
            cout << "location root: " << locationData.getRoot() << endl;
            cout << "location error log: " << locationData.getErrorLog() << endl;
            cout << "location default type: " << locationData.getDefaultType() << endl;
            cout << "location keepalive timeout: " << locationData.getKeepaliveTimeout() << endl;
            cout << "location header timeout: " << locationData.getHeaderTimeout() << endl;
            cout << "location client max body size: " << locationData.getClientMaxBodySize() << endl;
            cout << "location fastcgi pass: " << locationData.getFastcgiPass() << endl;
            cout << "location fastcgi index: " << locationData.getFastcgiIndex() << endl;
            cout << "location index: " << locationData.getIndex() << endl;
            cout << "location autoindex: " << locationData.getAutoindex() << endl;
            cout << "location access log: " << locationData.getAccessLog() << endl;
            cout << "location return: " << locationData.getReturn().first << " " << locationData.getReturn().second << endl;
            map<int, string> &errorPage = locationData.getErrorPage();
            for(map<int, string>::iterator it = errorPage.begin(); it != errorPage.end(); it++)
                cout << "location error page: " << it->first << " " << it->second << endl;
            map<string, string> &fastcgiParam = locationData.getFastcgiParam();
            for(map<string, string>::iterator it = fastcgiParam.begin(); it != fastcgiParam.end(); it++)
                cout << "location fastcgi param: " << it->first << " " << it->second << endl;
            vector<string> &allowedMethods = locationData.getAllowedMethods();
            for (size_t k = 0; k < allowedMethods.size(); k++)
                cout << "location allowed methods: " << allowedMethods[k] << endl;
            
            cout << "---------------------------------" << endl << endl;
        }
    }
}

int main()
{
    Directives::init();
    atexit(check);
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
    cout << "parser server num: " << parser.getServerConfig().size() << endl;
    Validator v(parser);
    HTTPServer *server = v.validate();
    
    test(server);
}