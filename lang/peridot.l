%{
    #include <string>
    #include <stack>
    #include "lexer.hpp"

    std::stack<int> indent_lvls;
    int current_line_indent_lvl = 0;
    bool dedenting = false;

    #define yyterminate() return Lang::Parser::make_END(*yylloc)

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
                /* We're doing multiple dedents, but we can't return multiple tokens.
                 * To do that, add fake lines with remaining indentation - we'll handle
                 * them here again. */
                unput('\n');
                for (int i = 0; i < indent_lvls.top(); i++) {
                    unput(' '); unput(' ');
                }
            } else {
                BEGIN(next_mode);
            }
            return Lang::Parser::make_DEDENT(*yylloc);
        } else {
            yyterminate();
        }
}
<line_indent>.       {
    if (!dedenting) unput(*yytext);
    yylloc->columns(-1);
    dedenting = false;
    if (current_line_indent_lvl > indent_lvls.top()) {
        std::cout << "--lex indent: current " << current_line_indent_lvl << " prev: " << indent_lvls.top() << std::endl;
        indent_lvls.push(current_line_indent_lvl);
        BEGIN(next_mode);
        return Lang::Parser::make_INDENT(*yylloc);
    } else if (current_line_indent_lvl < indent_lvls.top()) {
        indent_lvls.pop();
        if (current_line_indent_lvl != indent_lvls.top()) {
            /* Just like before, we need to return multiple DEDENT tokens. */
            for (int i = 0; i < current_line_indent_lvl; i++) {
                unput(' '); unput(' ');
            }
            unput('\n');
            /* Now we need to return to this block; insert a symbol
             * that will be caught by this rule:
             */
            unput(';');
            /* And make sure that we don't treat it as an actual token */
            dedenting = true;
            /* That allows us to treat one dedent at a time. Now unput the
             * rest so we can handle it later. */
            for (int i = 0; i < indent_lvls.top(); i++) {
                unput(' '); unput(' ');
            }
            unput('\n');
        } else {
            BEGIN(next_mode);
        }
        return Lang::Parser::make_DEDENT(*yylloc);
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
}

"if"   return Lang::Parser::make_K_IF(*yylloc);
"then" return Lang::Parser::make_K_THEN(*yylloc);
"else" return Lang::Parser::make_K_ELSE(*yylloc);
"do"   return Lang::Parser::make_K_DO(*yylloc);

[0-9]+ {
    const int val = atoi(yytext);
    return Lang::Parser::make_INT(val, *yylloc);
}

[a-z][A-Za-z0-9_]* {
    return Lang::Parser::make_REF(yytext, *yylloc);
}

"=" return Lang::Parser::make_ASSIGN(*yylloc);
"+" return Lang::Parser::make_PLUS(*yylloc);
"-" return Lang::Parser::make_MINUS(*yylloc);
"*" return Lang::Parser::make_STAR(*yylloc);
"/" return Lang::Parser::make_SLASH(*yylloc);
"(" return Lang::Parser::make_LPAR(*yylloc);
")" return Lang::Parser::make_RPAR(*yylloc);

[ ] { }

. {
    std::cerr << "Cannot translate " << yytext << " at " << *yylloc << std::endl;
}

%%


