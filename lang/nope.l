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

[a-z][A-Za-z0-9_]* {
    return Lang::Parser::make_REF(yytext, *yylloc);
}

"=" return Lang::Parser::make_ASSIGN(*yylloc);
"+" return Lang::Parser::make_PLUS(*yylloc);
"-" return Lang::Parser::make_MINUS(*yylloc);
"*" return Lang::Parser::make_STAR(*yylloc);
"/" return Lang::Parser::make_SLASH(*yylloc);

[ ]+ {
    /* whitespace; I'm not sure yet if I want to ignore it or.. */  
}

. {
    std::cerr << "Cannot translate " << yytext << " at " << *yylloc << std::endl;
}

%%

