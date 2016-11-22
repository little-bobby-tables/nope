%{
    #include <string>
    #include "lexer.hpp"

    #define yyterminate() return Lang::Parser::make_END(*yylloc)
%}

%option c++
%option yyclass="Lang::Lexer"
%option nodefault
%option noyywrap
%option debug

%%

%{
    yylloc->step();
%}

[0-9]+ {
    const int val = atoi(yytext);
    return Lang::Parser::make_INT(val, *yylloc);
}

%%
