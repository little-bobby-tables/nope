#ifndef LANG_AST
#define LANG_AST

#include "data.h"

namespace Lang {
    class ASTNode {
        public:
            virtual Value evaluate() = 0;
    };

    class ExpressionsNode : public ASTNode {
        public:
            ExpressionsNode() = default;
            Value evaluate() {
                return (Value)nullptr;
            }
    };

    class ProgramNode : public ASTNode {
        public:
            ProgramNode() = default;
            ProgramNode(std::string expressions) {
                this->program = expressions;
            }
            Value evaluate() {
                return (Value)(new std::string(this->program));
            }
        private:
            std::string program;
            /*
            ProgramNode(ExpressionsNode& program_node) {
                this->program = program_node;
            }
            Value evaluate() {
                return (Value)(new std::string("hi!"));
            }
        private:
            ExpressionsNode program;*/
    };
}

#endif

