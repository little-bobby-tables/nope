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
};

#define ST_OP_COUNT 6
const STOperator ST_OPS[] = {
    {"(", 1},
    {")", 1},
    {"+", 2},
    {"-", 2},
    {"*", 3},
    {"/", 3}
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

enum SRLeafRef {
    Reference,
    Value,
    Node
};

struct STLeaf {
    STLeafRef type;
    int ref;
};

struct STNode {
    int op;
    STLeaf left;
    STLeaf right;
};

struct Value {
    std::string val;
    std::string obj;
};

struct STExecScope {
    std::vector<Value> vals;
    std::vector<STNode> nodes;
    std::vector<std::string> refs;
};

#endif

