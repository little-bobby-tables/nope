#ifndef LANG_AST_VISITOR
#define LANG_AST_VISITOR

#include "ast.hpp"

namespace Lang {
    class ProgramASTVisitor : public ASTVisitor {
        public:
            void visit(ExpressionListNode* node);   
            void visit(ReferenceNode* node);
            void visit(ReferenceChainNode* node);
            void visit(AssignmentNode* node);
            void visit(FunctionDefinitionNode* node);
            void visit(GeneralCallNode* node);
            void visit(GeneralCallArgumentsNode* node);
            void visit(BlockNode* node);
            void visit(IfExpressionNode* node);
            void visit(InfixOpNode* node);
            void visit(IntegerValueNode* node);
            void visit(FloatValueNode* node);
    };
}

#endif

