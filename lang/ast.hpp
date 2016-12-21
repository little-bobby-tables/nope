#ifndef LANG_AST
#define LANG_AST

#include <memory>
#include "data.h"

#define value_to_string_ptr(value) (std::string*)(value)
#define value_to_string(value) *(value_to_string_ptr(value))
#define string_to_value(str) (Value)(new std::string(str))
#define release_pointers(vector) { for (size_t i = 0; i < vector.size(); i++) { delete vector[i]; } }

namespace Lang {
    typedef std::vector<std::string> ParameterList;

    class ASTNode {
        public:
            ASTNode() = default;
            virtual Value evaluate() = 0;
    };

    class ExpressionNode : public ASTNode {
        public:
            Value evaluate() { return 0; }
    };

    class ExpressionListNode : public ASTNode {
        public:
            ~ExpressionListNode() { release_pointers(this->expressions); }
            void push(ExpressionNode* node) { this->expressions.push_back(node); }
            Value evaluate() {
                std::string* out = new std::string();
                for (int i = 0; i < this->expressions.size(); i++) {
                    *out += value_to_string(this->expressions[i]->evaluate());
                }
                return (Value)(out);
            }
        private:
            std::vector<ExpressionNode*> expressions;
    };

    class ReferenceNode : public ExpressionNode {
        public:
            ReferenceNode(std::string ref) {
                this->reference = ref;
            }
            Value evaluate() {
                return string_to_value(this->reference);
            }
        private:
            std::string reference;
    };

    class ReferenceChainNode : public ExpressionNode {
        public:
            ~ReferenceChainNode() { release_pointers(this->chain); }

            void append(ExpressionNode* ref) { this->chain.push_back(ref); }

            Value evaluate() {
                std::string out;
                for (int i = 0; i < chain.size(); i++) {
                    std::string* ev = value_to_string_ptr(chain[i]->evaluate());
                    out += *ev;
                    delete ev;
                }
                return string_to_value(out);
            }
        private:
            std::vector<ExpressionNode*> chain;
    };

    class AssignmentNode : public ExpressionNode {
        public:
            AssignmentNode(ReferenceChainNode* left, ExpressionNode* right) { 
                this->left = left;
                this->right = right;
            }
            ~AssignmentNode() {
                delete this->left; delete this->right;
            }
            Value evaluate() {
                std::string* left_v = value_to_string_ptr(this->left->evaluate());
                std::string* right_v = value_to_string_ptr(this->right->evaluate());
                std::string out = *left_v + *right_v;
                delete left_v; delete right_v;
                return string_to_value(out); 
            }
        private:
            ReferenceChainNode* left;
            ExpressionNode* right;
    };

    class FunctionDefinitionNode : public ExpressionNode {
        public:
            FunctionDefinitionNode(ParameterList params, ExpressionNode* body) {
                this->params = params;
                this->body = body;
            }
            ~FunctionDefinitionNode() { delete this->body; }
            Value evaluate() {
                std::string* body_v = value_to_string_ptr(this->body->evaluate());
                std::string out = "(fn";
                for (int i = 0; i < this->params.size(); i++) {
                    out += this->params[i];
                }
                out += " -> " + *body_v + ")";
                delete body_v;
                return string_to_value(out); 
            }
        private:
            ParameterList params;
            ExpressionNode* body;
    };

    class GeneralCallArgumentsNode : public ExpressionListNode { };

    class GeneralCallNode : public ExpressionNode {
        public:
            GeneralCallNode(ReferenceChainNode* target, GeneralCallArgumentsNode* args) {
                this->target = target;
                this->args = args;
            }
            ~GeneralCallNode() { delete this->target; delete this->args; }
            Value evaluate() {
                return string_to_value("general_call_node");
            }
        private:
            ReferenceChainNode* target;
            GeneralCallArgumentsNode* args;
    };

    class BlockNode : public ExpressionNode {
        public:
            BlockNode(ExpressionListNode* node) {
                this->expressions = node;
            }
            ~BlockNode() { delete this->expressions; }
            Value evaluate() {
                return expressions->evaluate();
            }
        private:
            ExpressionListNode* expressions;
    };

    class IfExpressionNode : public ExpressionNode {
        public:
            IfExpressionNode(ExpressionNode* condition, ExpressionNode* then) {
                this->condition = condition;
                this->then = then;
                this->otherwise = nullptr;
            }
            IfExpressionNode(ExpressionNode* condition, ExpressionNode* then, ExpressionNode* otherwise) {
                this->condition = condition;
                this->then = then;
                this->otherwise = otherwise;
            }
            ~IfExpressionNode() { delete this->condition; delete this->then; delete this->otherwise; }

            Value evaluate() {
                return string_to_value("if_node");
            }
        private:
            ExpressionNode* condition;
            ExpressionNode* then;
            ExpressionNode* otherwise;
    };

    class InfixOpNode : public ExpressionNode {
        public:
            InfixOpNode(ExpressionNode* left, ExpressionNode* right, std::string op) {
                this->left = left;
                this->right = right;
                this->op = op;
            }
            ~InfixOpNode() { delete this->left; delete this->right; }
            Value evaluate() { return string_to_value(op + " op node"); }
        private:
            ExpressionNode* left;
            ExpressionNode* right;
            std::string op;
    };

    class ValueNode : public ExpressionNode {};

    class IntegerValueNode : public ValueNode {
        public:
            IntegerValueNode(long value) {
                this->value = value;
            }
            Value evaluate() {
                return string_to_value(std::to_string(this->value));
            }
        private:
            long value;
    };

    class FloatValueNode : public ValueNode {
        public:
            FloatValueNode(double value) {
                this->value = value;
            }
            Value evaluate() { 
                return string_to_value(std::to_string(this->value));
            }
        private:
            long value;
    };
}

#endif

