#ifndef LANG_LEXER
#define LANG_LEXER

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "parser.tab.hh"
#include "location.hh"

namespace Lang {
    class Lexer : public yyFlexLexer {
        public:
            Lexer(std::istream *in) : yyFlexLexer(in) {
                yylloc = new Lang::Parser::location_type();
            };
            virtual ~Lexer() {
                delete yylloc;
            };

            virtual Lang::Parser::symbol_type yylex(Lang::Parser::semantic_type* const yylval, Lang::Parser::location_type *yylloc);

         private:
           Lang::Parser::semantic_type *yylval = nullptr;
           Lang::Parser::location_type *yylloc = nullptr;
     };
};

#endif
