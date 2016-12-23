#ifndef VM_INSTRUCTION_CODES
#define VM_INSTRUCTION_CODES

#include <vector>
#include <string>
#include <cstddef>
#include <iostream>

namespace VM {
    typedef std::vector<unsigned char> ByteSequence;

    static const unsigned char i_send_msg_to_obj = 1;
    static const unsigned char i_put_int         = 2;
    static const unsigned char i_put_float       = 3;
}

#endif

