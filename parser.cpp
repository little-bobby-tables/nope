#include "parser.h"
#include "e/eval.h"
#include "c/loader.h"

#include <iostream>

using namespace std;

string parse(STExecScope &sc, queue<STToken> tokens) {
    build_scope(sc, tokens);
    load_protos(sc.protos);
    for (int i = 0; i < sc.vals.size(); i++) {
        cout << "Val " << sc.vals[i].val << " (" << sc.vals[i].obj << ")" <<  endl;
    }
    for (int i = 0; i < sc.nodes.size(); i++) {
        cout << "Node " << sc.nodes[i].op << " ";
        cout << "(left: " << sc.nodes[i].left.ref;
        if (sc.nodes[i].left.type == STLeafRef::Value) {
            cout << " Value)";
        } else {
            cout << " Node)";
        }
        cout << "(right: " << sc.nodes[i].right.ref;
        if (sc.nodes[i].right.type == STLeafRef::Value) {                cout << " Value)";                                     } else {                                                        cout << " Node)";                                       }
        cout << endl;
    }
    CValue c = eval_ast(sc);
    cout << endl << "=> " << c.val << "(" << c.obj << ")" << endl;
    return "";
}

void add_node(vector<STNode> &nodes, stack<STLeaf> &operands, string op) {
    STLeaf right = operands.top(); operands.pop();
    STLeaf left = operands.top(); operands.pop();
    
    nodes.push_back({ op, left, right });
    int index = nodes.size() - 1;
    operands.push({ index, STLeafRef::Node });
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
        case STTokenType::Reference:
            sc.vals.push_back({ token.val, "Reference" });
            break;
    }
    return sc.vals.size() - 1;
}

STExecScope build_scope(STExecScope &sc, queue<STToken> tokens) {
    stack<STOperator> operators;
    stack<STLeaf> operands;

    while (!tokens.empty()) {
        STToken tk = tokens.front(); tokens.pop();
        if (tk.type == STTokenType::Operator) {
            STOperator op = ST_OPS[to_i(tk.val)];
            if (operators.empty()) {
                operators.push(op);
            } else {
                if (op.val == ")") {
                    while (operators.top().val != "(") {
                        add_node(sc.nodes, operands, operators.top().val);
                        operators.pop();
                    }
                    operators.pop();
                } else {
                    while (!operators.empty() && operators.top().prec > op.prec) {
                        add_node(sc.nodes, operands, operators.top().val);
                        operators.pop();
                    }
                    operators.push(op);
                }
            }
        } else {
            int index = record_token(tk, sc);
            operands.push({ index, STLeafRef::Value });
        }
    }
    if (operators.empty()) {
        sc.nodes.push_back({ "", operands.top(), { 0, STLeafRef::Empty }});
    }
    while (!operators.empty()) {
        add_node(sc.nodes, operands, operators.top().val);
        operators.pop();
    }

    return sc;
}

