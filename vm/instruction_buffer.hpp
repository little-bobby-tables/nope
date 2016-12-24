#ifndef VM_INSTRUCTION_BUFFER
#define VM_INSTRUCTION_BUFFER

#include "instruction_codes.hpp"

namespace VM {
    class InstructionBuffer {
        public:
            ByteSequence to_byte_sequence() {
                return buffer;
            }
            void push(const byte i) {
                buffer.push_back(i);
                instruction_count++;
            }
            void push(long int_value) {
                pack_8byte_value(int_value);
            }
            void push(double float_value) {
                pack_8byte_value(float_value);
            }
            void push(std::string string_value) {
                const char* c = string_value.c_str();
                while (*c) {
                    buffer.push_back(*c++);
                };
                buffer.push_back('\0');
            }
            size_t size() {
                return instruction_count;
            }
        private:
            template<typename T>
            inline void pack_8byte_value(T v) {
                union { T value; byte packed[8]; } _conversion;
                _conversion.value = v;
                for (int i = 0; i < 8; i++) {
                    buffer.push_back(_conversion.packed[i]);
                }
            }

            ByteSequence buffer;
            size_t instruction_count = 0;
    };
}

#endif

