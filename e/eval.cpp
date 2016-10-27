#include "eval.h"
#include "../c/loader.h"

using namespace std;

CValue eval_ast(STExecScope &sc) {
    cout << "reached eval_ast" << endl;
    return eval_node(sc, sc.nodes[sc.nodes.size() - 1]);
}

CValue eval_node(STExecScope &sc, STNode node) {
    CValue left, right;
    if (node.left.type == STLeafRef::Value) {
        left = sc.vals[node.left.ref];
    } else {
        left = eval_node(sc, sc.nodes[node.left.ref]);
    }
    if (node.right.type == STLeafRef::Value) {
        right = sc.vals[node.right.ref];
    } else {
        right = eval_node(sc, sc.nodes[node.right.ref]);
    }
    cout << "vals evaluated" << endl;
    CFunction f = find_func(sc, node.op, find_proto(sc, left.obj));
    return f.body(left, right);
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
        cout << "traversed " << proto.funcs[i].name << endl;
        if (proto.funcs[i].name == name) {
            return proto.funcs[i];
        }
    }
    cout << "did not fund a func" << endl;
    if (proto.base != "") {
        return find_func(sc, name, find_proto(sc, proto.base));
    }
    return find_func(sc, "no_method", sc.protos[0]);
}

