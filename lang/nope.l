%{
    #include <string>
    #include "lex.hpp"

    #undef  YY_DECL
    #define YY_DECL Lang::Parse::symbol_type Lang::Lex::yylex(Lang::Parse::semantic_type* const lval, Lang::Parse::location_type *loc)

    #define yyterminate() return Lang::Parse::make_END(*loc)
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
  const int val = atoi(yytext);
  return Lang::Parse::make_INT(val, *loc);
}

%%
