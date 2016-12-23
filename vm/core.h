#ifndef VM_CORE
#define VM_CORE

#include "core_data.hpp"
#include "instruction_sequence.hpp"

namespace VM {
    class Core {
        public:
            Core(InstructionSequence instructions) {
                this->iseq = instructions;
            }
            void loop();
        private:
            ValueStack val_stack;
            InstructionSequence iseq;
    };
}

#endif

