%skeleton "lalr1.cc"
%require "3.0.4"

%defines
%debug
%error-verbose
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

%token END 0 "end of input"

%token <int>           INT
%token <std::string>   REF

%token B_START B_END END
%token ASSIGN PLUS MINUS STAR SLASH LPAR RPAR

%type <std::string>    asgn
%type <std::string>    stmts
%type <std::string>    stmt
%type <std::string>    expr
%type <std::string>    op

%left '+' '-'
%left '*' '/'

%start program

%%

program : stmts {
            std::cout << $1 << std::endl; 
            core.parsing_finished($1);
        }
        ;

stmts   : stmt { $$ = $1; }
        | stmts stmt
        | stmts block
        ;

block   : B_START stmts B_END
        ;

stmt    : asgn { $$ = $1; }
        | expr { $$ = $1; }
        ;

expr    : REF {
            std::cout << "Ref " << $1 << std::endl;
            $$ = $1;
        }
        | INT {
            std::cout << "Int " << $1 << std::endl;
            $$ = std::to_string($1);
        }
        | expr op expr {
            std::cout << "Expr " << $1 << $2 << $3 << std::endl;
            $$ = $1 + $2 + $3;
        }
        ;

asgn    : REF ASSIGN expr {
            std::cout << "assigning " << $3 << " to " << $1 << std::endl;
        }
        ;

op      : PLUS { $$ = "+"; }
        | MINUS { $$ = "-"; }
        | STAR { $$ = "*"; }
        | SLASH { $$ = "/"; }
        ;

%%

void Lang::Parser::error(const location_type& l, const std::string& error) {
   std::cerr << "Error: " << error << " at " << l << std::endl;
}

