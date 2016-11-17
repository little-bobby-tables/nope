%{
    #include <string>
    #include "lex.hpp"

    #undef  YY_DECL
    #define YY_DECL int Lang::Lex::yylex( Lang::Parse::semantic_type * const lval, Lang::Parse::location_type *loc )

    using t = Lang::Parse::token;

%}

%option c++
%option yyclass="Lang::Lex"
%option nodefault
%option noyywrap
%option debug

%%

%{
    yylval = lval;
%}

[0-9]+ { 
    yyval.inum = atoi(yytext);
    return t::INT;
}

. {
    return yytext;
}

%%

