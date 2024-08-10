%option noyywrap
%option never-interactive

%{
#include <stdio.h>

#define T_FLOAT     1
#define T_INT       2
#define T_PLUS      3
#define T_MINUS     4
#define T_MULTIPLY  5
#define T_DIVIDE    6
#define T_LEFT      7
#define T_RIGHT     8
#define T_STRING    9
#define T_LBRACKET  10
#define T_RBRACKET  11
#define T_IPV4      12
#define T_SEMICOLON 13
#define T_BYTES     14
#define T_TIME      15
#define T_METHOD    16
#define T_STATCODE  17

double  fval;
int     ival;
char    unit;
%}

%%
[ \t\n]+                    {}
#.*                         {}
(10[0-3]|20[0-8]|214|226|30[0-8]|40[0-9]|41[0-8]|42[0-9]|43[14]|45[01]|49[7-9]|50[0-9]|51[01]|52[1235]|530|599) {return T_STATCODE;}
^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?) { return T_IPV4; }
";"                         { return T_SEMICOLON; }
[0-9]+                      {ival = atoi(yytext); return T_INT;}
[0-9]+[kKmMgG]              {ival = atoi(yytext); unit = yytext[strlen(yytext) - 1];  return T_BYTES;}
[0-9]+([smhd]|ms)           {ival = atoi(yytext); unit = yytext[strlen(yytext) - 1];  return T_TIME;}
(GET|HEAD|POST|PUT|DELETE|CONNECT|OPTIONS|TRACE) {return T_METHOD;}
\".*\"                      { return T_STRING; }
\{                          { return T_LBRACKET; }
\}                          { return T_RBRACKET; }
[^ \t\n\r;\{\}]+            { return T_STRING; }
.                           { return T_STRING; }

%%

int main(int argc, char** argv) {
    FILE *file = stdin;

    file = fopen(argv[1], "r");

    yyin = file;

    int result = yylex();
    while (result)
    {
        printf("%s\n tag:[%d]\n", yytext, result);
        result = yylex();
    }
}