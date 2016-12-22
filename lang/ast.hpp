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

    class ASTVisitor;

    class ASTNode {
        public:
            ASTNode() = default;
            virtual void accept(ASTVisitor& visitor) = 0;
    };

    class ExpressionNode : public ASTNode {};
    class ValueNode      : public ExpressionNode {};

    class ExpressionListNode;
    class ReferenceNode;
    class ReferenceChainNode;
    class AssignmentNode;
    class FunctionDefinitionNode;
    class GeneralCallNode;
    class GeneralCallArgumentsNode;
    class BlockNode;
    class IfExpressionNode;
    class InfixOpNode;

    class IntegerValueNode;
    class FloatValueNode;

    class ASTVisitor {
        public:
            virtual void visit(ExpressionListNode* node)       = 0;
            virtual void visit(ReferenceNode* node)            = 0;
            virtual void visit(ReferenceChainNode* node)       = 0;
            virtual void visit(AssignmentNode* node)           = 0;
            virtual void visit(FunctionDefinitionNode* node)   = 0;
            virtual void visit(GeneralCallNode* node)          = 0;
            virtual void visit(GeneralCallArgumentsNode *node) = 0;
            virtual void visit(BlockNode* node)                = 0;
            virtual void visit(IfExpressionNode* node)         = 0;
            virtual void visit(InfixOpNode* node)              = 0;
            virtual void visit(IntegerValueNode* node)         = 0;
            virtual void visit(FloatValueNode* node)           = 0;
    };

    class ExpressionListNode : public ASTNode {
        public:
            std::vector<ExpressionNode*> expressions;

            ~ExpressionListNode() {
                release_pointers(this->expressions);
            }
            void push(ExpressionNode* node) {
                this->expressions.push_back(node);
            }
            void accept(ASTVisitor& v) { v.visit(this); }
    };

    class ReferenceNode : public ExpressionNode {
        public:
            std::string reference;

            ReferenceNode(std::string ref) {
                this->reference = ref;
            }
            void accept(ASTVisitor& v) { v.visit(this); }
    };

    class ReferenceChainNode : public ExpressionNode {
        public:
            std::vector<ExpressionNode*> chain;

            ~ReferenceChainNode() {
                release_pointers(this->chain);
            }
            void append(ExpressionNode* ref) {
                this->chain.push_back(ref);
            }
            void accept(ASTVisitor& v) { v.visit(this); }
    };

    class AssignmentNode : public ExpressionNode {
        public:
            ReferenceChainNode* left;
            ExpressionNode* right;

            AssignmentNode(ReferenceChainNode* left, ExpressionNode* right) {
                this->left = left;
                this->right = right;
            }
            ~AssignmentNode() {
                delete this->left; delete this->right;
            }
            void accept(ASTVisitor& v) { v.visit(this); }
    };

    class FunctionDefinitionNode : public ExpressionNode {
        public:
            ParameterList params;
            ExpressionNode* body;

            FunctionDefinitionNode(ParameterList params, ExpressionNode* body) {
                this->params = params;
                this->body = body;
            }
            ~FunctionDefinitionNode() {
                delete this->body;
            }
            void accept(ASTVisitor& v) { v.visit(this); }
    };

    class GeneralCallArgumentsNode : public ExpressionListNode {};

    class GeneralCallNode : public ExpressionNode {
        public:
            ReferenceChainNode* target;
            GeneralCallArgumentsNode* args;

            GeneralCallNode(ReferenceChainNode* target, GeneralCallArgumentsNode* args) {
                this->target = target;
                this->args = args;
            }
            ~GeneralCallNode() {
                delete this->target;
                delete this->args;
            }
            void accept(ASTVisitor& v) { v.visit(this); }
    };

    class BlockNode : public ExpressionNode {
        public:
            ExpressionListNode* expressions;

            BlockNode(ExpressionListNode* node) {
                this->expressions = node;
            }
            ~BlockNode() {
                delete this->expressions;
            }
            void accept(ASTVisitor& v) { v.visit(this); }
    };

    class IfExpressionNode : public ExpressionNode {
        public:
            ExpressionNode* condition;
            ExpressionNode* then;
            ExpressionNode* otherwise;

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
            ~IfExpressionNode() {
                delete this->condition;
                delete this->then;
                delete this->otherwise;
            }
            void accept(ASTVisitor& v) { v.visit(this); }
    };

    class InfixOpNode : public ExpressionNode {
        public:
            ExpressionNode* left;
            ExpressionNode* right;
            std::string op;

            InfixOpNode(ExpressionNode* left, ExpressionNode* right, std::string op) {
                this->left = left;
                this->right = right;
                this->op = op;
            }
            ~InfixOpNode() {
                delete this->left;
                delete this->right;
            }
            void accept(ASTVisitor& v) { v.visit(this); }
    };

    class IntegerValueNode : public ValueNode {
        public:
            long value;

            IntegerValueNode(long value) {
                this->value = value;
            }
            void accept(ASTVisitor& v) { v.visit(this); }
    };

    class FloatValueNode : public ValueNode {
        public:
            double value;

            FloatValueNode(double value) {
                this->value = value;
            }
            void accept(ASTVisitor& v) { v.visit(this); }
    };
}

#endif

