#ifndef LANG_DRIVER
#define LANG_DRIVER

/*#include <cctype>
#include <cassert>*/

#include <string>
#include <cstddef>
#include <istream>

#include "lexer.hpp"
#include "parser.tab.hh"

/* A convenience class wrapping lexing and parsing. */
namespace Lang {
    class Driver {
        public:
            Driver() = default;
            ~Driver() {
                delete lexer;
                delete parser;
            }
            void parse(std::istream &stream) {
                if (!stream.good() && stream.eof()) return;
                delete lexer;
                delete parser;
                lexer = new Lang::Lexer(&stream);
                parser = new Lang::Parser(*lexer, *this);
                
                int result = parser->parse();
                if (result != 0) {
                    /* TODO: is this necessary? parser's already reporting the errors it encounters on its own */
                }
            }
        private:
            Lang::Lexer* lexer = nullptr;
            Lang::Parser* parser = nullptr;
    };
};

#endif
