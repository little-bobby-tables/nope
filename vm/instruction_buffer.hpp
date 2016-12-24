#ifndef VM_INSTRUCTION_BUFFER
#define VM_INSTRUCTION_BUFFER

#include "instruction_codes.hpp"

namespace VM {
    class InstructionBuffer {
        public:
            ByteSequence to_byte_sequence() {
                return buffer;
            }
            void push(const unsigned char i) {
                buffer.push_back(i);
                instruction_count++;
            }
            void push(long int_value) {
                /* FIXME: ugly & not portable (doesn't take endianness into account) */
                /* gets the job done tho */
                buffer.push_back(static_cast<int>(int_value & 0xFF));
                buffer.push_back(static_cast<int>((int_value >> 8) & 0xFF));
                buffer.push_back(static_cast<int>((int_value >> 16) & 0xFF));
                buffer.push_back(static_cast<int>((int_value >> 24) & 0xFF));
                buffer.push_back(static_cast<int>((int_value >> 32) & 0xFF));
                buffer.push_back(static_cast<int>((int_value >> 40) & 0xFF));
                buffer.push_back(static_cast<int>((int_value >> 48) & 0xFF));
                buffer.push_back(static_cast<int>((int_value >> 56) & 0xFF));
            }
            void push(double float_value) {

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
            ByteSequence buffer;
            size_t instruction_count = 0;
    };
}

#endif

