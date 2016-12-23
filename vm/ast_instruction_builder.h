#ifndef VM_AST_INSTRUCTION_BUILDER
#define VM_AST_INSTRUCTION_BUILDER

#include "../lang/ast.hpp"
#include "instruction_buffer.hpp"

namespace VM {
    class InstructionBuilderASTVisitor : public Lang::ASTVisitor {
        public:
            InstructionBuffer ast_to_instructions(Lang::ExpressionListNode* program);
            void visit(Lang::ExpressionListNode* node);
            void visit(Lang::ReferenceNode* node);
            void visit(Lang::ReferenceChainNode* node);
            void visit(Lang::AssignmentNode* node);
            void visit(Lang::FunctionDefinitionNode* node);
            void visit(Lang::GeneralCallNode* node);
            void visit(Lang::GeneralCallArgumentsNode* node);
            void visit(Lang::BlockNode* node);
            void visit(Lang::IfExpressionNode* node);
            void visit(Lang::InfixOpNode* node);
            void visit(Lang::IntegerValueNode* node);
            void visit(Lang::FloatValueNode* node);
        private:
            InstructionBuffer instructions;
    };
}

#endif

