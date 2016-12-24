#ifndef VM_CORE_DATA
#define VM_CORE_DATA

#include <stack>
#include <cstdint>

namespace VM {
    typedef uintptr_t Value;
    typedef std::stack<Value> ValueStack;
    
    /* Value is largely inspired by the current implementation of MRI (CRuby),
     * which itself was based on techniques employed in Lisp implementation,
     * where it is usually treated as a pointer to an object, but may hold a
     * special value to speed up execution (tagged pointer). */

    /* With data aligned to its size (a multiple of 8-byte word for x86-64),
     * we have the last 3 bits of an address always being 000, which can be used
     * to store bit tags. */

    static const int v_integer_mask = 0b1;
    static const int v_integer_flag = 0b1;
    inline Value int_to_value(long i) { return ((Value) (i)) << 1 | v_integer_flag; }
    inline long value_to_int(Value v) { return ((long) (v)) >> 1; }
    inline bool is_v_int(Value v) { return (v & v_integer_mask) == v_integer_flag; }

    /* Based on MRI's Flonum
     * (thorough explanation: http://www.atdot.net/~ko1/activities/SapporoRubyKaigi2012_ko1_pub.pdf,
     *  implementation: https://github.com/ruby/ruby/commit/b3b5e626ad69bf22be3228f847f94e1b68f40888) */
    static const int v_float_mask = 0b11;
    static const int v_float_flag = 0b10;
    inline Value bitwise_rotate_left(Value v, int by) { return (((v) << (by)) | ((v) >> ((64 - by)))); }
    inline Value bitwise_rotate_right(Value v, int by) { return (((v) >> (by)) | ((v) << ((64 - by)))); }
    inline Value float_to_value(double f) {
        if (f == 0.0) return 0b010;
        union { double f; Value v; } _type_punning_float_representation;
        _type_punning_float_representation.f = f;
        return (bitwise_rotate_left(_type_punning_float_representation.v, 3) & ~0b1) | v_float_flag;
    }
    inline double value_to_float(Value v) {
        if (v == 0b010) return 0.0;
        union { double f; Value v; } _type_punning_float_representation;
        Value sign_bit = (v >> 63);
        _type_punning_float_representation.v =
            bitwise_rotate_right(((sign_bit ^ 1) << 1) | sign_bit | (v & ~0b11), 3);
        return _type_punning_float_representation.f;
    }
    inline bool is_v_float(Value v) { return (v & v_float_mask) == v_float_flag; }

    /* 0 is not a valid pointer and evaluates to false in C++ */
    static const Value False = 0b0;
    /* Other constant values can be represented using the last bit: */
    static const Value True = 0b1100;
    static const Value Nothing = 0b10100;
}

#endif

