#ifndef VM_LOOP
#define VM_LOOP

#include <stack>

#include "../core/data.hpp"
#include "dispatcher.hpp"
#include "instruction_sequence.hpp"

namespace VM {
    class Loop {
        public:
            Loop(InstructionSequence instructions) {
                iseq = instructions;
                dispatcher = std::make_unique<Dispatcher>();
            }
            void run();
        private:
            std::unique_ptr<Dispatcher> dispatcher;
            std::stack<Core::Value> v_stack;
            InstructionSequence iseq;
    };
}

#endif

