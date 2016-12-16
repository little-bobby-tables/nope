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

%token END 0    "end of input"
%token INDENT   "indentation"
%token UNINDENT "unindentation"
%token NEWLINE  "new line"

%token <int>           INT
%token <std::string>   REF

%token ASSIGN PLUS MINUS STAR SLASH LPAR RPAR
%token K_DO K_IF K_THEN K_ELSE

%type <std::string>    expressions
%type <std::string>    expression
%type <std::string>    assignment
%type <std::string>    if_statement
%type <std::string>    block
%type <std::string>    value
%type <std::string>    op

%left '+' '-'
%left '*' '/'

%start program

%%

program
    : expressions {
        core.parsing_finished($1);
    }
    ;

expressions
    : expression terminator {
        $$ = $1;
    }
    | expressions expression terminator {
        $$ = $1 + $2;
    }
    ;

expression
    : assignment {
        $$ = $1;
    }
    | expression op expression {
        $$ = "(expr " + $1 + " " + $2 + " " + $3 + ")";
    }
    | if_statement {
        $$ = $1;
    }
    | value {
        $$ = $1;
    }
    | REF {
        $$ = "(ref " + $1 + ")";
    }
    ;

assignment
    : REF ASSIGN expression {
        $$ = "(asgn " + $1 + " = " + $3 + ")";
    }
    ;

block
    : terminator INDENT expressions UNINDENT {
        $$ = "(block " + $3 + ")";
    }
    ;

if_statement
    : K_IF expression K_THEN expression {
        $$ = "(if " + $2 + " then " + $4 + ")";
    }
    | K_IF expression K_THEN block {
        $$ = "(if " + $2 + " then " + $4 + ")";
    }
    ;

value
    : INT {
        $$ = "(int " + std::to_string($1) + ")";
    }
    ;

op
    : PLUS { $$ = "+"; }
    | MINUS { $$ = "-"; }
    | STAR { $$ = "*"; }
    | SLASH { $$ = "/"; }
    ;

terminator
    : NEWLINE
    | END
    ;

%%

void Lang::Parser::error(const location_type& l, const std::string& error) {
   std::cerr << "Error: " << error << " at " << l << std::endl;
}

