#include "data.h"

#ifndef EVALUATOR_H
#define EVALUATOR_H

class Evaluator {
    public:
        Evaluator(STExecScope *scope);
        CValue eval();
        void clear_scope();
    private:
        CValue eval_node(STNode node);
        CValue get_property(std::string name);
        void set_property(std::string name, CValue val);
        CPrototype find_proto(std::string name);
        CFunction find_func(std::string name, CPrototype proto);
        STExecScope *sc;
        std::vector<CPrototype> protos;
};

#endif

