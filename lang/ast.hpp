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
            void push_expression(std::string expression) {
                this->expressions.push_back(expression);
            }
            Value evaluate() {
                std::string *out = new std::string();
                for (int i = 0; i < this->expressions.size(); i++) {
                    *out += this->expressions[i];
                }
                return (Value)(out);
            }
        private:
            std::vector<std::string> expressions;
    };
}

#endif

