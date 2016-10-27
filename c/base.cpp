#include "base.h"

CPrototype base_proto() {
    std::vector<CFunction> funcs;
    funcs.push_back({ "no_method", "Base", &no_method });
    return { "Base", "", funcs };
}

CValue no_method(CValue left, CValue right) {
    return { "Method " + left.val + " not found in the prototype chain", "Error" };
}

