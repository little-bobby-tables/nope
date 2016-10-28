#include "../data.h"

#ifndef E_EVAL
#define E_EVAL

CValue eval_ast(STExecScope &sc);
CValue eval_node(STExecScope &sc, STNode node);

CValue get_property(STExecScope &sc, std::string name);
void set_property(STExecScope &sc, std::string name, CValue val);

CPrototype find_proto(STExecScope &sc, std::string name);
CFunction find_func(STExecScope &sc, std::string name, CPrototype proto);

#endif
