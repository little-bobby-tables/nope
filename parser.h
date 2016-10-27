#include "data.h"

std::string parse(std::queue<STToken> tokens);
void add_node(std::vector<STNode> &nodes, std::stack<STLeaf> &operands, std::string op);
int record_token(STToken token, STExecScope &sc);
STExecScope to_scope(std::queue<STToken> tokens);

