#include "integer.h"

CPrototype integer_proto() {
    std::vector<CFunction> funcs;
    funcs.push_back({ "+", "Integer", &add });
    funcs.push_back({ "-", "Integer", &subtract });
    funcs.push_back({ "*", "Integer", &multiply });
    funcs.push_back({ "/", "Integer", &divide });
    return { "Integer", "Base", funcs };
}

CValue add(CValue left, CValue right) {
    int l = to_i(left.val);
    int r = to_i(right.val);
    return { to_str(l + r), "Integer" };
}

CValue subtract(CValue left, CValue right) {
    int l = to_i(left.val);
    int r = to_i(right.val);
    return { to_str(l - r), "Integer" };
}

CValue multiply(CValue left, CValue right) {
    int l = to_i(left.val);
    int r = to_i(right.val);
    return { to_str(l * r), "Integer" };
}

CValue divide(CValue left, CValue right) {
    int l = to_i(left.val);
    int r = to_i(right.val);
    if (r == 0) return { "Division by zero", "Error" };
    return { to_str(l / r), "Integer" };
}


