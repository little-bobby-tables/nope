#include <cctype>
#include <fstream>
#include <cassert>

#include "mc_driver.hpp"

Lang::Core::~Core()
{
    delete lexer;
    scanner = nullptr;
    delete parser;
    parser = nullptr;
}

void Lang::Core::parse(std::istream &stream) {
    if (!stream.good() && stream.eof()) {
        return;
    } else
        delete lexer;
        lexer = new Lang::Lex(&stream);
        delete parser;
        parser = new Lang::Parse(*lexer, *this);                }
    if (parser->parse() != 0) {
        /* TODO: handle error */
    }
}

