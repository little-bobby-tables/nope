%skeleton "lalr1.cc"
%require "3.0.4"

%debug
%defines
%define api.namespace { Lang }
%define parser_class_name { Parse }

%code requires {
    #include <string>

    namespace Lang {
        class Lex;
        class Core;
    };
}

%parse-param { Lex &lex }
%parse-param { Core &core }

%code {
   #include <iostream>
   #include <cstdlib>

   #include "core.h"

   #undef yylex
   #define yylex lex.yylex
}

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%token <int>           INT
%token <std::string>   REF
%token B_START B_END END

%type <std::string>    expr

%left '+' '-'
%left '*' '/'

%locations

%%

program : stmt;

stmts   : stmts stmt
        | stmts block
        ;

block   : B_START stmts B_END
        ;

stmt    : asgn
        ;

expr    : REF
        | INT
        | expr op expr
        ;

asgn    : REF '=' expr {
            std::cout << "assigning " << $3 << " to " << $1 << std::endl;
        }
        ;

op      : '+'
        | '-'
        | '*'
        | '/'
        ;

%%

void Lang::Parse::error(const location_type& l, const std::string& error) {
   std::cerr << "Error: " << error << " at " << l << std::endl;
}
