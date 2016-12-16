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

%token ASSIGN "="
%token PLUS   "+"
%token MINUS  "-"
%token STAR   "*"
%token SLASH  "/"
%token LPAR   "("
%token RPAR   ")"
%token K_DO K_IF K_THEN K_ELSE

%type <std::string>    expressions
%type <std::string>    expression infix_expression
%type <std::string>    assignment
%type <std::string>    if_statement
%type <std::string>    block
%type <std::string>    value

%nonassoc LPAR RPAR
%left ASSIGN
%left PLUS MINUS
%left STAR SLASH

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
    | infix_expression {
        $$ = $1;
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

infix_expression
    : LPAR expression RPAR {
        $$ = "(expr " + $2 + ")";
    }
    | expression PLUS expression {
        $$ = "(expr " + $1 + " + " + $3 + ")";
    }
    | expression MINUS expression {
        $$ = "(expr " + $1 + " - " + $3 + ")";
    }
    | expression STAR expression {
        $$ = "(expr " + $1 + " * " + $3 + ")";
    }
    | expression SLASH expression {
        $$ = "(expr " + $1 + " / " + $3 + ")";
    }

value
    : INT {
        $$ = "(int " + std::to_string($1) + ")";
    }
    ;

terminator
    : NEWLINE
    | END
    ;

%%

void Lang::Parser::error(const location_type& l, const std::string& error) {
   std::cerr << "Error: " << error << " at " << l << std::endl;
}

