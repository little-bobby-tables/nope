#ifndef VM_INSTRUCTION_SET
#define VM_INSTRUCTION_SET

#include "instruction_codes.hpp"

namespace VM {
    class InstructionSequence {
        public:
            InstructionSequence() = default;
            InstructionSequence(ByteSequence instructions) {
                this->seq = instructions;
            }
            unsigned char next() {
                if (position < seq.size() - 1) {
                    return seq[position++];
                } else {
                    return '\0';
                }
            }
            long get_int_value() {
                long value =
                    static_cast<long>(seq[position++])       |
                    static_cast<long>(seq[position++]) << 8  |
                    static_cast<long>(seq[position++]) << 16 |
                    static_cast<long>(seq[position++]) << 24 |
                    static_cast<long>(seq[position++]) << 32 |
                    static_cast<long>(seq[position++]) << 40 |
                    static_cast<long>(seq[position++]) << 48 |
                    static_cast<long>(seq[position++]) << 56 ;
                return value;
            }
            std::string get_str_value() {
                std::string str;
                char buff;
                while (buff = seq[position++]) {
                    str.push_back(buff);
                }
                return str;
            }
        private:
            size_t position = 0;
            ByteSequence seq;
    };
}

#endif

