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
            byte next() {
                if (position < seq.size() - 1) {
                    return seq[position++];
                } else {
                    return '\0';
                }
            }
            long get_int_value() {
                return unpack_8byte_value<long>();
            }
            double get_float_value() {
                return unpack_8byte_value<double>();
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
            template<typename T>
            inline T unpack_8byte_value() {
                union { T value; byte packed[8]; } conversion;
                for (int i = 0; i < sizeof(long); i++) {
                    conversion.packed[i] = seq[position++];
                }
                return conversion.value;
            }

            size_t position = 0;
            ByteSequence seq;
    };
}

#endif

