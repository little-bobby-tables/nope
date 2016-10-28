#include "eval.h"
#include "../c/loader.h"

using namespace std;

CValue eval_ast(STExecScope &sc) {
    cout << "reached eval_ast" << endl;
    return eval_node(sc, sc.nodes[sc.nodes.size() - 1]);
}

CValue eval_node(STExecScope &sc, STNode node) {
    CValue left, right;
    bool short_circuit = false;
    if (node.left.type == STLeafRef::Value) {
        left = sc.vals[node.left.ref];
    } else {
        left = eval_node(sc, sc.nodes[node.left.ref]);
    }
    if (node.right.type == STLeafRef::Value) {
        right = sc.vals[node.right.ref];
    } else if (node.right.type == STLeafRef::Node) {
        right = eval_node(sc, sc.nodes[node.right.ref]);
    } else {
        short_circuit = true;
    }
    
    if (!short_circuit && right.obj == "Reference") {
        right = get_property(sc, right.val);
    }
    if (left.obj == "Reference") {
        if (node.op == "=") {
            set_property(sc, left.val, right);
            return right;
        }
        left = get_property(sc, left.val);
    }
    
    if (short_circuit) return left;

    CFunction f = find_func(sc, node.op, find_proto(sc, left.obj));
    if (f.name == "no_method") left.val = node.op;
    return f.body(left, right);
}


CValue get_property(STExecScope &sc, std::string name) {
    for (int i = 0; i < sc.locals.size(); i++) {
        if (sc.locals[i].name == name) {
            return sc.locals[i].val;
        }
    }
    return { "Undefined local variable " + name, "Error" };
}

void set_property(STExecScope &sc, std::string name, CValue val) {
    CProperty prop = { name, val };
    for (int i = 0; i < sc.locals.size(); i++) {
        if (sc.locals[i].name == name) {
            sc.locals[i] = prop;
            return;
        }
    }
    sc.locals.push_back(prop);
}

CPrototype find_proto(STExecScope &sc, std::string name) {
    for (int i = 0; i < sc.protos.size(); i++) {
        if (sc.protos[i].name == name) {
            return sc.protos[i];
        }
    }
    return sc.protos[0];
}

CFunction find_func(STExecScope &sc, std::string name, CPrototype proto) {
    for (int i = 0; i < proto.funcs.size(); i++) {
        if (proto.funcs[i].name == name) {
            return proto.funcs[i];
        }
    }
    if (proto.base != "") {
        return find_func(sc, name, find_proto(sc, proto.base));
    }
    return find_func(sc, "no_method", sc.protos[0]);
}

