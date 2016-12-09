%skeleton "lalr1.cc"
%require "3.0.4"

%defines
%debug
%define api.namespace { Lang }
%define parser_class_name { Parser }

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%parse-param { Lexer &lexer }
%parse-param { Core &core }

%code requires {
    #include <string>

    namespace Lang {
        class Lexer;
        class Core;
    };
}

%locations

%code top {
    #include <iostream>
    #include <cstdlib>

    #include "core.h"

    #define yylex lexer.LEXER_YYLEX
}

%token <int>           INT
%token <std::string>   REF

%token B_START B_END END
%token ASSIGN PLUS MINUS STAR SLASH LPAR RPAR

%type <std::string>    expr
%type <std::string>    op

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
        | expr
        ;

expr    : REF {
            std::cout << "Ref " << $1 << std::endl;
        }
        | INT
        | expr op expr {
            std::cout << "Expr " << $1 << $2 << $3 << std::endl;
        }
        ;

asgn    : REF ASSIGN expr {
            std::cout << "assigning " << $3 << " to " << $1 << std::endl;
        }
        ;

op      : PLUS
        | MINUS
        | STAR
        | SLASH
        ;

%%

void Lang::Parser::error(const location_type& l, const std::string& error) {
   std::cerr << "Error: " << error << " at " << l << std::endl;
}

