#include <string>
#include <vector>
#include <stack>
#include <queue>

#include "utils.h"

#ifndef DATA_H
#define DATA_H

struct STOperator {
    std::string val;
    int prec;
    bool ltr;
};

#define ST_OP_COUNT 6
const STOperator ST_OPS[] = {
    { "(", 1, true },
    { ")", 1, true },
    { "+", 2, true },
    { "-", 2, true },
    { "*", 3, true },
    { "/", 3, true },
    { "!", 4, false }
};

enum STTokenType {
    Operator,
    Reference,
    Integer,
    Float,
    String
};

struct STToken {
    std::string val;
    STTokenType type;
};

enum STLeafRef {
    Value,
    Node
};

struct STLeaf {
    int ref;
    STLeafRef type;
};

struct STNode {
    std::string op;
    STLeaf left;
    STLeaf right;
};

struct CValue {
    std::string val;
    std::string obj;
};

struct CFunction {
    std::string name;
    std::string obj;
    CValue (*body)(CValue, CValue);
};

struct CPrototype {
    std::string name;
    std::string base;
    std::vector<CFunction> funcs;
};

struct STExecScope {
    std::vector<CValue> vals;
    std::vector<STNode> nodes;
    std::vector<CPrototype> protos;
};

#endif

