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

%token EndOfInput 0
%token Indentation Unindentation Newline

%token <int>           Integer
%token <std::string>   Reference

%token Assignment Addition Subtraction Multiplication Division
%token LeftParenthesis RightParenthesis Comma
%token Do If Then Else

%type <std::string>    expressions
%type <std::string>    expression infix_expression
%type <std::string>    function_call function_call_args
%type <std::string>    assignment
%type <std::string>    if_statement
%type <std::string>    block
%type <std::string>    value

%nonassoc LeftParenthesis RightParenthesis
%left Assignment
%left Addition Subtraction
%left Multiplication Division

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
    | function_call {
        $$ = $1;
    }
    | value {
        $$ = $1;
    }
    | Reference {
        $$ = "(ref " + $1 + ")";
    }
    ;

assignment
    : Reference Assignment expression {
        $$ = "(asgn " + $1 + " = " + $3 + ")";
    }
    ;

function_call
    : Reference LeftParenthesis function_call_args RightParenthesis {
        $$ = "(fcall " + $1 + " with args " + $3 + ")";
    }
    ;

function_call_args
    : function_call_args Comma expression {
        $$ = $1 + $3;
    }
    | expression {
        $$ = $1;
    }
    ;

block
    : terminator Indentation expressions Unindentation {
        $$ = "(block " + $3 + ")";
    }
    ;

if_statement
    : If expression Then expression {
        $$ = "(if " + $2 + " then " + $4 + ")";
    }
    | If expression Then block {
        $$ = "(if " + $2 + " then " + $4 + ")";
    }
    | If expression Then expression Else expression {
        $$ = "(if " + $2 + " then " + $4 + " else " + $6 + ")";
    }
    | If expression Then expression Else block {
        $$ = "(if " + $2 + " then " + $4 + " else " + $6 + ")";
    }
    | If expression Then block terminator Else expression {
        $$ = "(if " + $2 + " then " + $4 + " else " + $7 + ")";
    }
    | If expression Then block terminator Else block {
        $$ = "(if " + $2 + " then " + $4 + " else " + $7 + ")";
    }
    ;

infix_expression
    : LeftParenthesis expression RightParenthesis {
        $$ = "(expr " + $2 + ")";
    }
    | expression Addition expression {
        $$ = "(expr " + $1 + " + " + $3 + ")";
    }
    | expression Subtraction expression {
        $$ = "(expr " + $1 + " - " + $3 + ")";
    }
    | expression Multiplication expression {
        $$ = "(expr " + $1 + " * " + $3 + ")";
    }
    | expression Division expression {
        $$ = "(expr " + $1 + " / " + $3 + ")";
    }

value
    : Integer {
        $$ = "(int " + std::to_string($1) + ")";
    }
    ;

terminator
    : Newline
    | EndOfInput
    ;

%%

void Lang::Parser::error(const location_type& l, const std::string& error) {
   std::cerr << "Error: " << error << " at " << l << std::endl;
}

