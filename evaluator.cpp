#include "evaluator.h"
#include "c/loader.h"

using namespace std;

Evaluator::Evaluator(STExecScope *scope) {
    this->sc = scope;
    load_protos(this->protos);
}

CValue Evaluator::eval() {
    return eval_node(sc->nodes.back());
}

void Evaluator::clear_scope() {
    this->sc->nodes.clear();
    this->sc->vals.clear();
}

CValue Evaluator::eval_node(STNode node) {
    CValue left, right;
    bool short_circuit = false;
    if (node.left.type == STLeafRef::Value) {
        left = this->sc->vals[node.left.ref];
    } else {
        left = eval_node(this->sc->nodes[node.left.ref]);
    }
    if (node.right.type == STLeafRef::Value) {
        right = this->sc->vals[node.right.ref];
    } else if (node.right.type == STLeafRef::Node) {
        right = eval_node(this->sc->nodes[node.right.ref]);
    } else {
        short_circuit = true;
    }

    if (!short_circuit && right.obj == "Reference") {
        right = get_property(right.val);
    }
    if (left.obj == "Reference") {
        if (node.op == "=") {
            set_property(left.val, right);
            return right;
        }
        left = get_property(left.val);
    }

    if (short_circuit) return left;

    CFunction f = find_func(node.op, find_proto(left.obj));
    if (f.name == "no_method") {
        left.val = node.op;
    }
    return f.body(left, right);
}

CValue Evaluator::get_property(string name) {
    for (int i = 0; i < this->sc->locals.size(); i++) {
        if (this->sc->locals[i].name == name) {
            return this->sc->locals[i].val;
        }
    }
    return { "Undefined local variable " + name, "Error" };
}

void Evaluator::set_property(string name, CValue val) {
    CProperty prop = { name, val };
    for (int i = 0; i < this->sc->locals.size(); i++) {
        if (this->sc->locals[i].name == name) {
            this->sc->locals[i] = prop;
            return;
        }
     }
     this->sc->locals.push_back(prop);
}

CPrototype Evaluator::find_proto(string name) {
    for (int i = 0; i < this->protos.size(); i++) {
        if (this->protos[i].name == name) {
            return this->protos[i];
        }
    }
    return this->protos[0];
}

CFunction Evaluator::find_func(string name, CPrototype proto) {
    for (int i = 0; i < proto.funcs.size(); i++) {
        if (proto.funcs[i].name == name) {
             return proto.funcs[i];
        }
    }
    if (proto.base != "") {
        return find_func(name, find_proto(proto.base));
    }
    return find_func("no_method", this->protos[0]);
}

