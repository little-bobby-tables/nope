#include <iostream>
#include "ast_instruction_builder.h"

using namespace Lang;

VM::InstructionBuffer
VM::InstructionBuilderASTVisitor::ast_to_instructions (ExpressionListNode* program) {
    program->accept(*this);
    return this->instructions;
}

void VM::InstructionBuilderASTVisitor::visit(ExpressionListNode* node) {
    for (size_t i = 0; i < node->expressions.size(); i++) {
        node->expressions[i]->accept(*this);
    }
}

void VM::InstructionBuilderASTVisitor::visit(ReferenceNode* node) {
    instructions.push(VM::i_put_reference);
    instructions.push(node->reference);
}

void VM::InstructionBuilderASTVisitor::visit(ReferenceChainNode* node) {}

void VM::InstructionBuilderASTVisitor::visit(AssignmentNode* node) {
    /* Value is the first value popped off the stack, reference is the second */
    node->value->accept(*this);
    instructions.push(VM::i_assign_reference);
    instructions.push(node->name);
}

void VM::InstructionBuilderASTVisitor::visit(FunctionDefinitionNode* node) {}
void VM::InstructionBuilderASTVisitor::visit(GeneralCallNode* node) {}
void VM::InstructionBuilderASTVisitor::visit(GeneralCallArgumentsNode* node) {}
void VM::InstructionBuilderASTVisitor::visit(BlockNode* node) {}
void VM::InstructionBuilderASTVisitor::visit(IfExpressionNode* node) {}

void VM::InstructionBuilderASTVisitor::visit(InfixOpNode* node) {
    node->left->accept(*this);
    node->right->accept(*this);
    instructions.push(VM::i_send_msg_to_obj);
    instructions.push(node->op);
}

void VM::InstructionBuilderASTVisitor::visit(IntegerValueNode* node) {
    instructions.push(VM::i_put_int);
    instructions.push(node->value);
}

void VM::InstructionBuilderASTVisitor::visit(FloatValueNode* node) {
    instructions.push(VM::i_put_float);
    instructions.push(node->value);
}
  
