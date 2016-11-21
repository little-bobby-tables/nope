%skeleton "lalr1.cc"
%require "3.0.4"

%debug
%defines
%locations
%define api.namespace { Lang }
%define parser_class_name { Parser }

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%parse-param { Lexer &lex }
%parse-param { Core &core }

%code requires {
    #include <string>

    namespace Lang {
        class Lexer;
        class Core;
    };
}

%code {
   #include <iostream>
   #include <cstdlib>

   #include "core.h"

   #define YY_DECL Lang::Parser::symbol_type Lang::Lexer::yylex(semantic_type* const yylval, location_type *yylloc)

   #define yylex lex.yylex
}

%token <int>           INT
%token <std::string>   REF
%token B_START B_END END

%type <std::string>    expr

%left '+' '-'
%left '*' '/'

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

void Lang::Parser::error(const location_type& l, const std::string& error) {
   std::cerr << "Error: " << error << " at " << l << std::endl;
}
