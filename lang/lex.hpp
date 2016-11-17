#ifndef LANG_LEX
#define LANG_LEX

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "parser.tab.hh"
#include "location.hh"

namespace Lang {
    class Lex : public yyFlexLexer {
        public:
            Lex(std::istream *in) : yyFlexLexer(in) {
                loc = new Lang::Parse::location_type();
            };
            virtual ~Lex() {
                delete loc;
            };

             using FlexLexer::yylex;
             virtual int yylex(Lang::Parse::semantic_type * const lval, Lang::Parse::location_type *location );

         private:
             Lang::Parse::semantic_type *yylval = nullptr;
             Lang::Parse::location_type *loc    = nullptr;
     }
}
#endif

