#include "ast_visitor.h"
#include <iostream>

using namespace Lang;

void ProgramASTVisitor::visit(ExpressionListNode* node) {
    std::cout << "expression_list_node" << std::endl;
    for (size_t i = 0; i < node->expressions.size(); i++) {
        node->expressions[i]->accept(*this);
    }
}

void ProgramASTVisitor::visit(ReferenceNode* node) {}
void ProgramASTVisitor::visit(ReferenceChainNode* node) {}
void ProgramASTVisitor::visit(AssignmentNode* node) {}
void ProgramASTVisitor::visit(FunctionDefinitionNode* node) {}
void ProgramASTVisitor::visit(GeneralCallNode* node) {}
void ProgramASTVisitor::visit(GeneralCallArgumentsNode* node) {}
void ProgramASTVisitor::visit(BlockNode* node) {}
void ProgramASTVisitor::visit(IfExpressionNode* node) {}
void ProgramASTVisitor::visit(InfixOpNode* node) {}
void ProgramASTVisitor::visit(IntegerValueNode* node) {}
void ProgramASTVisitor::visit(FloatValueNode* node) {}
  
