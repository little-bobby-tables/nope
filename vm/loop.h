#ifndef VM_LOOP
#define VM_LOOP

#include "../core/data.hpp"
#include "instruction_sequence.hpp"

namespace VM {
    class Loop {
        public:
            Loop(InstructionSequence instructions) {
                this->iseq = instructions;
            }
            void run();
        private:
            std::stack<Core::Value> val_stack;
            InstructionSequence iseq;
    };
}

#endif

