#include "parser.h"

using namespace std;

string parse(stack<STToken> tokens) {
    return to_st(tokens);
}

void add_node(stack<STNode> &nodes) {
    STNode right = nodes.front();
    tokens.pop();
    STNode left = nodes.front();
    tokens.pop();
}

int record_token(STToken token, STExecScope &sc) {
    switch (token.type) {
        case STTokenType::Integer:
            sc.vals.push_back({ token.val, "Integer" });
            break;
        case STTokenType::Float:
            sc.vals.push_back({ token.val, "Float" });
            break;
        case STTokenType::String:
            sc.vals.push_back({ token.val, "String" });
            break;
    }
    return sc.vals.size() - 1;
}

STExecScope to_scope(stack<STToken> tokens) {
    STExecScope sc;
    stack<OP> operators;
    string expr;
   
    for (int i = 0; i < raw.length(); i++) {
        int op_i = op_index(raw, i);
        if (op_i != -1) {
            if (operators.empty()) {
                operators.push(OPS[op_i]);
            } else {
                while (!operators.empty() && operators.top().prec > OPS[op_i].prec) {
                    expr += operators.top().val;
                    operators.pop();
                }
                operators.push(OPS[op_i]);
            }
        } else if (raw.at(i) == ')') {
            while (operators.top().val != "(") {
                expr += operators.top().val;
                operators.pop();
            }
            operators.pop();
        } else if (raw.at(i) != ' ') {
            expr.push_back(raw.at(i));
        }
    }
    while (!operators.empty()) {
        expr += operators.top().val;
        operators.pop();
    }
    return expr;
}

