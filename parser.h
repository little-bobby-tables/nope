#include "data.h"

#ifndef PARSER_H
#define PARSER_H

class Parser {
    public:
        Parser(STExecScope *scope);
        void parse_to_scope(std::queue<STToken> tokens);
    private:
        void build_ast(std::queue<STToken> tokens);
        void add_node(std::stack<STLeaf> &operands, std::string op);
        int record_token(STToken token);
        STExecScope *sc;
};

#endif

