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

%token INDENT DEDENT

%token B_START B_END END
%token ASSIGN PLUS MINUS STAR SLASH LPAR RPAR
%token K_DO K_IF K_THEN K_ELSE

%type <std::string>    asgn
%type <std::string>    stmts
%type <std::string>    if_stmt
%type <std::string>    block
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
        | stmts stmt { $$ = $1 + $2; }
        ;

block   : INDENT stmts DEDENT {
            $$ = "(block " + $2 + ")";
        }
        ;

stmt    : asgn { $$ = $1; }
        | expr { $$ = $1; }
        | if_stmt { $$ = $1; }
        ;

expr    : REF {
            $$ = "(ref " + $1 + ")";
        }
        | INT {
            $$ = "(int " + std::to_string($1) + ")";
        }
        | expr op expr {
            $$ = "(expr " + $1 + " " + $2 + " " + $3 + ")";
        }
        ;

asgn    : REF ASSIGN expr {
            std::cout << "assigning " << $3 << " to " << $1 << std::endl;
        }
        ;

if_stmt : K_IF stmt K_THEN stmt {
            $$ = "(if with stmt " + $4 + " and condition " + $2 + ")";
        }
        | K_IF stmt K_THEN block { 
            $$ = "(if with block " + $4 + " and condition " + $2 + ")";
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

