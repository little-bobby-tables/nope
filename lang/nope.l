%{
    #include <string>
    #include "lexer.hpp"

    #define YY_DECL Lang::Parser::symbol_type Lang::Lexer::yylex(Lang::Parser::semantic_type* const lval, Lang::Parser::location_type *loc)

    #define yyterminate() return Lang::Parser::make_END(*loc)
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
    return Lang::Parser::make_INT(val, *loc);
}

%%
