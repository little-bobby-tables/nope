#ifndef VM_CORE_DATA
#define VM_CORE_DATA

#include <stack>
#include <cstdint>

namespace VM {
    typedef uintptr_t Value;
    typedef std::stack<Value> ValueStack;

    /* See int_to_value definition */
    static const int v_integer_flag = 0x01;
    
    /* (Taken from MRI (C Ruby), which itself was inspired by Lisp:
     * Objects are generally arranged on addresses multiple of 4, so
     * odd numbers can be reversed to efficiently store
     * (sizeof(Value) * 8 - 1) integers. */
    inline Value int_to_value(long i) {
        return ((Value) (i)) << 1 | v_integer_flag;
    }

    inline long value_to_int(Value v) {
        return ((long) (v)) >> 1;
    }
}

#endif

