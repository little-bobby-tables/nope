#include "integer.h"

CPrototype integer_proto() {
    std::vector<CFunction> funcs;
    funcs.push_back({ "+", "Integer", &add });
    return { "Integer", "BaseObject", funcs };
}

CValue add(CValue left, CValue right) {
    int l = to_i(left.val);
    int r = to_i(right.val);
    return { to_str(l + r), "Integer" };
}

