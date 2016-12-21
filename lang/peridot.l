%{
    #include <string>
    #include <stack>
    #include "lexer.hpp"

    std::stack<int> indent_lvls;
    int current_line_indent_lvl = 0;
    bool unindenting_multiple_lvls = false;

    #define yyterminate() return Lang::Parser::make_EndOfInput(*yylloc)
    #define return_token(token) return Lang::Parser::make_##token(*yylloc)
    #define return_value_token(token, val) return Lang::Parser::make_##token(val, *yylloc)

    #define YY_USER_INIT { indent_lvls.push(0); BEGIN(initial_line_indent); }
    #define YY_USER_ACTION { yylloc->columns(yyleng); }
%}

%option c++
%option yyclass="Lang::Lexer"
%option nodefault
%option noyywrap
%option debug

%x initial_line_indent
%x line_indent
%s normal

%%

%{
    int next_mode = normal;
    yylloc->step();
%}

<initial_line_indent>. {
    yylloc->columns(-1);
    next_mode = normal;
    yyless(0);
    BEGIN(line_indent);
}
<line_indent>"  "    {
    current_line_indent_lvl++;
}
<line_indent>\n      {
    current_line_indent_lvl = 0;
}
<line_indent><<EOF>> {
     if (indent_lvls.top() != 0) {
            indent_lvls.pop();
            if (current_line_indent_lvl != indent_lvls.top()) {
                /* We're doing multiple unindents, but we can't return multiple tokens.
                 * To do that, add fake lines with remaining indentation - we'll handle
                 * them here again. */
                unput('\n');
                for (int i = 0; i < indent_lvls.top(); i++) {
                    unput(' '); unput(' ');
                }
            } else {
                BEGIN(next_mode);
            }
            return Lang::Parser::make_Unindentation(*yylloc);
        } else {
            yyterminate();
        }
}
<line_indent>.       {
    if (!unindenting_multiple_lvls) unput(*yytext);
    yylloc->columns(-1);
    unindenting_multiple_lvls = false;
    if (current_line_indent_lvl > indent_lvls.top()) {
        indent_lvls.push(current_line_indent_lvl);
        BEGIN(next_mode);
        return Lang::Parser::make_Indentation(*yylloc);
    } else if (current_line_indent_lvl < indent_lvls.top()) {
        indent_lvls.pop();
        if (current_line_indent_lvl != indent_lvls.top()) {
            unindenting_multiple_lvls = true;
            /* Treat one unindent at a time */
            for (int i = 0; i < current_line_indent_lvl; i++) {
                unput(' '); unput(' ');
            }
            unput('\n');
            /* Insert fake symbol to return to this action... */
            unput(';');
            /* ...and handle the rest of the unindent */
            for (int i = 0; i < indent_lvls.top(); i++) {
                unput(' '); unput(' ');
            }
            unput('\n');
        } else {
            BEGIN(next_mode);
        }
        /* By design, unindentation is expected to be followed by the Newline token */
        for (int i = 0; i < indent_lvls.top(); i++) {
            unput(' '); unput(' ');
        }
        unput('\n');
        return Lang::Parser::make_Unindentation(*yylloc);
    } else {
        /* Indentation unchanged */
        BEGIN(next_mode);
    }
}
<normal>\n {
    yylloc->lines(1);
    current_line_indent_lvl = 0;
    next_mode = YY_START;
    BEGIN(line_indent);
    return Lang::Parser::make_Newline(*yylloc);
}

"if"     return_token(If);
"then"   return_token(Then);
"else"   return_token(Else);
"do"     return_token(Do);
"fn"     return_token(Fn);
"class"  return_token(Class);
"method" return_token(Method);
"new"    return_token(New);

[0-9]+ {
    const long val = atol(yytext);
    return_value_token(Integer, val);
}

[0-9]+[.][0-9]+ {
    const double val = atof(yytext);
    return_value_token(Float, val);
}

[a-z][A-Za-z0-9_]* {
    return_value_token(Reference, yytext);
}

(([A-Z][A-Z]?|[0-9]+[A-Z]?)[a-z]+)+ {
    return_value_token(ClassReference, yytext);
}

"=" return_token(Assignment);
"+" return_token(Addition);
"-" return_token(Subtraction);
"*" return_token(Multiplication);
"/" return_token(Division);
"(" return_token(LeftParenthesis);
")" return_token(RightParenthesis);
"," return_token(Comma);
"." return_token(Accessor);
"?" return_token(WildcardElement);
"->" return_token(Arrow);

[ ] { }

. {
    std::cerr << "Cannot translate " << yytext << " at " << *yylloc << std::endl;
}

%%


