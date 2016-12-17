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
%token <float>         Float
%token <std::string>   Reference

%token Assignment Addition Subtraction Multiplication Division
%token LeftParenthesis RightParenthesis Accessor Comma
%token Do If Then Else

%type <std::string>    expressions
%type <std::string>    expression infix_expression dynamic_expression
%type <std::string>    reference_chain
%type <std::string>    function_call function_call_args
%type <std::string>    assignment
%type <std::string>    if_statement
%type <std::string>    block
%type <std::string>    value

%nonassoc LeftParenthesis RightParenthesis
%left Accessor
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
    | dynamic_expression {
        $$ = $1;
    }
    | if_statement {
        $$ = $1;
    }
    | reference_chain {
        $$ = "(ref " + $1 + ")";
    }
    ;

assignment
    : Reference Assignment expression {
        $$ = "(asgn " + $1 + " = " + $3 + ")";
    }
    ;

dynamic_expression
    : LeftParenthesis expression RightParenthesis {
        $$ = "(" + $2 + ")";
    }
    | function_call {
        $$ = $1;
    }
    | value {
        $$ = $1;
    }
    ;

reference_chain
    : reference_chain Accessor Reference {
        $$ = $1 + " -> " + $3;
    }
    | dynamic_expression Accessor Reference {
        $$ = $1 + " -> " + $3;
    }
    | Reference {
        $$ = $1;
    }
    ;

function_call
    : Reference LeftParenthesis function_call_args RightParenthesis {
        $$ = "(call " + $1 + " with args " + $3 + ")";
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
    : expression Addition expression {
        $$ = "(" + $1 + " + " + $3 + ")";
    }
    | expression Subtraction expression {
        $$ = "(" + $1 + " - " + $3 + ")";
    }
    | expression Multiplication expression {
        $$ = "(" + $1 + " * " + $3 + ")";
    }
    | expression Division expression {
        $$ = "(" + $1 + " / " + $3 + ")";
    }

value
    : Integer {
        $$ = "(int " + std::to_string($1) + ")";
    }
    | Float {
        $$ = "(float " + std::to_string($1) + ")";
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

