#include "parser.h"

using namespace std;

Parser::Parser(STExecScope *scope) {
    this->sc = scope;
}

void Parser::parse_to_scope(queue<STToken> tokens) {
    build_ast(tokens);
}

void Parser::build_ast(queue<STToken> tokens) {
    stack<STOperator> operators;
    stack<STLeaf> operands;

    while (!tokens.empty()) {
        STToken tk = tokens.front(); tokens.pop();
        if (tk.type == STTokenType::Operator) {
            STOperator op = ST_OPS[to_i(tk.val)];
            if (operators.empty()) {
                operators.push(op);
            } else {
                if (op.val == "(") {
                    operators.push(op);
                } else if (op.val == ")") {
                    while (operators.top().val != "(") {
                        add_node(operands, operators.top().val);
                        operators.pop();
                    }
                    operators.pop();
                } else {
                    while (!operators.empty() && operators.top().prec > op.prec) {
                        add_node(operands, operators.top().val);
                        operators.pop();
                    }
                    operators.push(op);
                }
            }
        } else {
            int index = record_token(tk);
            operands.push({ index, STLeafRef::Value });
        }
    }
    if (operators.empty()) {
        this->sc->nodes.push_back({ "", operands.top(), { 0, STLeafRef::Empty }});
    }
    while (!operators.empty()) {
         add_node(operands, operators.top().val);
         operators.pop();
    }
}

void Parser::add_node(stack<STLeaf> &operands, string op) {
    STLeaf right = operands.top(); operands.pop();
    STLeaf left = operands.top(); operands.pop();

    this->sc->nodes.push_back({ op, left, right });
    int index = this->sc->nodes.size() - 1;
    operands.push({ index, STLeafRef::Node });
}

int Parser::record_token(STToken token) {
    switch (token.type) {
        case STTokenType::Integer:
            this->sc->vals.push_back({ token.val, "Integer" });
            break;
        case STTokenType::Float:
            this->sc->vals.push_back({ token.val, "Float" });
            break;
        case STTokenType::String:
            this->sc->vals.push_back({ token.val, "String" });
            break;
        case STTokenType::Reference:
            this->sc->vals.push_back({ token.val, "Reference" });
            break;
     }
     return this->sc->vals.size() - 1;
}

