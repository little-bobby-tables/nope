#ifndef VM_DRIVER
#define VM_DRIVER

#include "../lang/ast.hpp"
#include "ast_instruction_builder.h"
#include "instruction_sequence.hpp"
#include "core.h"

namespace VM {
    class Driver {
        public:
            void process(Lang::ExpressionListNode* program) {
                InstructionBuilderASTVisitor v = InstructionBuilderASTVisitor();
                InstructionBuffer ins = v.ast_to_instructions(program);
                InstructionSequence iseq = InstructionSequence(ins.to_byte_sequence());
                Core exec_core = Core(iseq);
                exec_core.loop();
            }
    };
}

#endif

