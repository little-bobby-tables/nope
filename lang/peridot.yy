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
%token <std::string>   ClassReference

%token Class Method New
%token Assignment Addition Subtraction Multiplication Division
%token LeftParenthesis RightParenthesis 
%token Accessor Comma Arrow WildcardElement
%token Do If Then Else Fn

%type <std::string>    expressions
%type <std::string>    expression infix_expression
%type <std::string>    reference_chain
%type <std::string>    function_call function_definition
%type <std::string>    function_parameters parameter_list function_arguments argument_list
%type <std::string>    class_declaration class_block class_expression class_expressions
%type <std::string>    constructor_definition method_definition
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
    | reference_chain {
        $$ = "(ref " + $1 + ")";
    }
    | if_statement {
        $$ = $1;
    }
    | function_call {
        $$ = $1;
    }
    | function_definition {
        $$ = $1;
    }
    | value {
        $$ = $1;
    }
    | class_declaration {
        $$ = $1;
    }
    ;

assignment
    : reference_chain Assignment expression {
        $$ = "(asgn " + $1 + " = " + $3 + ")";
    }
    ;

reference_chain
    : reference_chain Accessor Reference {
        $$ = $1 + " . " + $3;
    }
    | expression Accessor Reference {
        $$ = $1 + " . " + $3;
    }
    | Reference {
        $$ = $1;
    }
    ;

function_definition
    : Fn function_parameters Arrow expression {
        $$ = "(fn " + $2 + " -> " + $4 + ")";
    }
    | Fn function_parameters Arrow block {
        $$ = "(fn " + $2 + " -> " + $4 + ")";
    }
    ;

function_parameters
    : LeftParenthesis parameter_list RightParenthesis {
        $$ = $2;
    }
    | LeftParenthesis RightParenthesis {
        $$ = "()";
    }
    ;

parameter_list
    : parameter_list Comma Reference {
        $$ = $1 + $3;
    }
    | Reference {
        $$ = $1;
    }
    ;

function_call
    : reference_chain function_arguments {
        $$ = "(call " + $1 + " with args " + $2 + ")";
    }
    ;

function_arguments
    : LeftParenthesis argument_list RightParenthesis {
        $$ = $2;
    }
    | LeftParenthesis RightParenthesis {
        $$ = "()";
    }
    ;

argument_list
    : argument_list Comma expression {
        $$ = $1 + $3;
    }
    | argument_list Comma WildcardElement {
        $$ = $1 + "(?)";
    }
    | expression {
        $$ = $1;
    }
    | WildcardElement {
        $$ = "(?)";
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
        $$ = "(" + $2 + ")";
    }
    | expression Addition expression {
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

/* OOP features */

class_expressions
    : class_expressions terminator class_expression {
        $$ = $1 + $3;
    }
    | class_expression {
        $$ = $1;
    }
    ;

class_expression
    : constructor_definition {
        $$ = $1;
    }
    | method_definition {
        $$ = $1;
    }
    ;

class_block
    : terminator Indentation class_expressions Unindentation {
        $$ = "(class_block " + $3 + ")";
    }
    ;

class_declaration
    : Class ClassReference class_block {
        $$ = "(class " + $2 + " " + $3 + ")";
    }

constructor_definition
    : New function_arguments block {
        $$ = "(constructor " + $2 + ": " + $3 + ")";
    }
    ;

method_definition
    : Method Reference block {
        $$ = "(method " + $2 + ": " + $3 + ")";
    }
    | Method Reference function_arguments block {
        $$ = "(method " + $2 + " (" + $3 + ") " + $4 + ")";
    }
    ;

%%

void Lang::Parser::error(const location_type& l, const std::string& error) {
   std::cerr << "Error: " << error << " at " << l << std::endl;
}

