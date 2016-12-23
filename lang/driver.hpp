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
            ExpressionListNode* parse(std::istream &stream) {
                if (!stream.good() && stream.eof()) return nullptr;
                delete lexer;
                delete parser;
                lexer = new Lang::Lexer(&stream);
                parser = new Lang::Parser(*lexer, *this);
                
                parser->parse();
                return this->parsed_ast;
            }
            ExpressionListNode* parsed_ast = nullptr;
        private:
            Lang::Lexer* lexer = nullptr;
            Lang::Parser* parser = nullptr;
    };
};

#endif
