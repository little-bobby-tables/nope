#ifndef VM_DRIVER
#define VM_DRIVER

#include "ast_instruction_builder.h"
#include "loop.h"

namespace VM {
    class Driver {
        public:
            void process(Lang::ExpressionListNode* program) {
                InstructionBuilderASTVisitor v = InstructionBuilderASTVisitor();
                InstructionBuffer ins = v.ast_to_instructions(program);
                InstructionSequence iseq = InstructionSequence(ins.to_byte_sequence());
                Loop exec_loop = Loop(iseq);
                exec_loop.run();
            }
    };
}

#endif

