%debug
%defines
%define api.namespace { Lang }
%define parser_class_name { Parse }

%code requires {
    namespace Lang {
        class Lex;
        class Core;
    }
}

%parse-param { Lex &lex }
%parse-param { Core &core }

%code{
   #include <iostream>
   #include <cstdlib>
   #include <fstream>
   
   #include "core.h"

   #undef yylex
   #define yylex lex.yylex
}

%union {
  int inum;
  std::string ref;
}

%define api.value.type variant
%define parse.assert

%token <inum>  INT
%token <ref>   REF
%token B_START, B_END

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

Lang::Parse::error( const location_type &l, const std::string &err_message ) {
   std::cerr << "Error: " << err_message << " at " << l << std::endl;
}

