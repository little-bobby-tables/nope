#ifndef LANG_CORE
#define LANG_CORE

#include <string>
#include <cstddef>
#include <istream>

#include "lexer.hpp"
#include "parser.tab.hh"

namespace Lang {
    class Core {
        public:
            Core() = default;
            virtual ~Core();
            void parsing_finished(std::string program);
            void parse(std::istream &stream);
        private:
            Lang::Parser *parser = nullptr;
            Lang::Lexer *lexer = nullptr;
    };
};

#endif
