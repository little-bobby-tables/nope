#ifndef LANG_DRIVER
#define LANG_DRIVER

/*#include <cctype>
#include <cassert>*/

#include <string>
#include <cstddef>
#include <istream>

#include "ast_visitor.h"
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
                
                parser->parse();
            }
            void parsing_finished(ExpressionListNode* program) {
                std::cout << "received node" << std::endl;
                ProgramASTVisitor v = ProgramASTVisitor();
                program->accept(v);
            }
        private:
            Lang::Lexer* lexer = nullptr;
            Lang::Parser* parser = nullptr;
    };
};

#endif
