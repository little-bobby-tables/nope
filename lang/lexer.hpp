#ifndef LANG_LEXER
#define LANG_LEXER

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "parser.tab.hh"
#include "location.hh"

/* It's impossible to override the return type of yylex (int) so we have
 * to use a different method there. */
#define YY_DECL Lang::Parser::symbol_type Lang::Lexer::yylex_sym()
/* Use this macro to call yylex from the parser */
#define LEXER_YYLEX yylex_sym

namespace Lang {
    class Lexer : public yyFlexLexer {
        public:
            Lexer(std::istream *in) : yyFlexLexer(in) {
                yylloc = new Lang::Parser::location_type();
            };
            virtual ~Lexer() {
                delete yylloc;
            };

            virtual Lang::Parser::symbol_type yylex_sym();

         private:
           Lang::Parser::semantic_type *yylval = nullptr;
           Lang::Parser::location_type *yylloc = nullptr;
     };
};

#endif
