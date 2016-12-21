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
    #include "ast.hpp"

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

%token <long>          Integer
%token <double>        Float
%token <std::string>   Reference
%token <std::string>   ClassReference

%token Class Method New
%token Assignment Addition Subtraction Multiplication Division
%token LeftParenthesis RightParenthesis 
%token Accessor Comma Arrow WildcardElement
%token Do If Then Else Fn

/* OOP features
%type <std::string>    class_declaration class_block class_expression class_expressions
%type <std::string>    constructor_definition method_definition
*/

%type <Lang::ExpressionListNode*>       expression_list
%type <Lang::ExpressionNode*>           expression
%type <Lang::ReferenceChainNode*>       reference_chain
%type <Lang::AssignmentNode*>           assignment
%type <Lang::FunctionDefinitionNode*>   function_definition
%type <Lang::ParameterList>             function_parameters parameter_list
%type <Lang::BlockNode*>                block
%type <Lang::GeneralCallArgumentsNode*> general_call_arguments general_call_argument_list
%type <Lang::GeneralCallNode*>          general_call
%type <Lang::IfExpressionNode*>         if_expression
%type <Lang::InfixOpNode*>              infix_op_expression
%type <Lang::ValueNode*>                value

%nonassoc LeftParenthesis RightParenthesis
%left Accessor
%left Assignment
%left Addition Subtraction
%left Multiplication Division

%start program

%%

program
    : expression_list {
        std::cout << *((std::string*)($1->evaluate())) << std::endl;
    }
    ;

expression_list
    : expression_list expression terminator {
        $$ = $1;
        $$->push($2);
    }
    | expression terminator {
        $$ = new ExpressionListNode();
        $$->push($1);
    }
    ;

expression
    : LeftParenthesis expression RightParenthesis { $$ = $2; }
    | assignment                                  { $$ = $1; }
    | infix_op_expression                         { $$ = $1; }
    | reference_chain                             { $$ = $1; }
    | if_expression                               { $$ = $1; }
    | general_call                                { $$ = $1; }
    | function_definition                         { $$ = $1; }
    | value                                       { $$ = $1; }
    ;
/* OOP features:
    | class_declaration   { $$ = $1; }
    ;
*/

assignment
    : reference_chain Assignment expression {
        $$ = new AssignmentNode($1, $3);
    }
    ;

reference_chain
    : reference_chain Accessor Reference {
        $$ = $1;
        $$->append(new ReferenceNode($3));
    }
    | expression Accessor Reference {
        $$ = new ReferenceChainNode();
        $$->append($1);
        $$->append(new ReferenceNode($3));           
    }
    | Reference {
        $$ = new ReferenceChainNode();
        $$->append(new ReferenceNode($1));
    }
    ;

function_definition
    : Fn function_parameters Arrow expression {
        $$ = new FunctionDefinitionNode($2, $4);
    }
    | Fn function_parameters Arrow block {
        $$ = new FunctionDefinitionNode($2, $4);
    }
    ;

function_parameters
    : LeftParenthesis parameter_list RightParenthesis {
        $$ = $2;
    }
    | LeftParenthesis RightParenthesis {
        $$ = ParameterList();
    }
    ;

parameter_list
    : parameter_list Comma Reference {
        $$ = $1;
        $1.push_back($3);
    }
    | Reference {
        $$ = ParameterList();
    }
    ;

general_call
    : reference_chain general_call_arguments {
        $$ = new GeneralCallNode($1, $2);
    }
    ;

general_call_arguments
    : LeftParenthesis general_call_argument_list RightParenthesis {
        $$ = $2;
    }
    | LeftParenthesis RightParenthesis {
        $$ = new GeneralCallArgumentsNode();
    }
    ;

general_call_argument_list
    : general_call_argument_list Comma expression {
        $$ = $1;
        $$->push($3);
    }
    | general_call_argument_list Comma WildcardElement {
        $$ = $1;
    }
    | expression {
        $$ = new GeneralCallArgumentsNode();
        $$->push($1);
    }
    | WildcardElement {
        $$ = new GeneralCallArgumentsNode();
    }
    ;

block
    : terminator Indentation expression_list Unindentation {
        $$ = new BlockNode($3);
    }
    ;

if_expression
    : If expression Then expression { 
        $$ = new IfExpressionNode($2, $4); 
    }
    | If expression Then block { 
        $$ = new IfExpressionNode($2, $4); 
    }
    | If expression Then expression Else expression { 
        $$ = new IfExpressionNode($2, $4, $6);
    }
    | If expression Then expression Else block {
        $$ = new IfExpressionNode($2, $4, $6);
    }
    | If expression Then block terminator Else expression {
        $$ = new IfExpressionNode($2, $4, $7);
    }
    | If expression Then block terminator Else block {
        $$ = new IfExpressionNode($2, $4, $7);
    }
    ;

infix_op_expression
    : expression Addition expression {
        $$ = new InfixOpNode($1, $3, "+");
    }
    | expression Subtraction expression {
        $$ = new InfixOpNode($1, $3, "-");
    }
    | expression Multiplication expression {
        $$ = new InfixOpNode($1, $3, "*");
    }
    | expression Division expression {
        $$ = new InfixOpNode($1, $3, "/");
    }

value
    : Integer {
        $$ = new IntegerValueNode($1);
    }
    | Float {
        $$ = new FloatValueNode($1);
    }
    ;

terminator
    : Newline
    | EndOfInput
    ;

/* OOP features

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
*/

%%

void Lang::Parser::error(const location_type& l, const std::string& error) {
   std::cerr << "Error: " << error << " at " << l << std::endl;
}

