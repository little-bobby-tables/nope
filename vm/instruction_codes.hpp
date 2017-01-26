#ifndef VM_INSTRUCTION_CODES
#define VM_INSTRUCTION_CODES

#include <vector>
#include <string>
#include <cstddef>
#include <iostream>

namespace VM {
    typedef unsigned char byte;
    typedef std::vector<byte> ByteSequence;

    static const byte i_send_msg_to_obj  = 1;
    static const byte i_put_int          = 2;
    static const byte i_put_float        = 3;
    static const byte i_put_reference    = 4;
    static const byte i_assign_reference = 5;
}

#endif

