#ifndef LANG_CORE
#define LANG_CORE

#include <string>
#include <cstddef>
#include <istream>

#include "lex.hpp"
#include "parser.tab.hh"

namespace Lang {
    class Core {
        public:
            Core() = default;
            virtual ~Core();
            void parse(std::istream &stream);
        private:
            Lang::Parse *parser = nullptr;
            Lang::Lex *lexer = nullptr;
    };
}

#endif
