#include <cctype>
#include <fstream>
#include <cassert>

#include "core.h"

Lang::Core::~Core()
{
    delete lexer;
    lexer = nullptr;
    delete parser;
    parser = nullptr;
}

void Lang::Core::parsing_finished(std::string program) {
    std::cout << "Received parsed result " << program << std::endl;
}

void Lang::Core::parse(std::istream &stream) {
    if (!stream.good() && stream.eof()) {
        return;
    } else {
        delete lexer;
        lexer = new Lang::Lexer(&stream);
        delete parser;
        parser = new Lang::Parser(*lexer, *this);
    }
    if (parser->parse() != 0) {
        /* TODO: handle error */
    }
}
