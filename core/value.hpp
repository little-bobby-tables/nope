#ifndef CORE_VALUE
#define CORE_VALUE

#include <cstdint>

namespace Core {
    typedef uintptr_t Value;
      
    /* Value is largely inspired by the current implementation of MRI (CRuby),
     * which itself was based on techniques employed in Lisp implementation,
     * where it is usually treated as a pointer to an object, but may hold a
     * special value to speed up execution (tagged pointer). */

    /* With data aligned to its size (a multiple of 8-byte word for x86-64),
     * we have the last 3 bits of an address always being 000, which can be used
     * to store bit tags. */

    static const int v_integer_mask = 0b1;
    static const int v_integer_flag = 0b1;
    inline Value v_from_int(long i) { return ((Value) (i)) << 1 | v_integer_flag; }
    inline long v_to_int(Value v) { return ((long) (v)) >> 1; }
    inline bool v_is_int(Value v) { return (v & v_integer_mask) == v_integer_flag; }

    /* Based on MRI's Flonum
     * (thorough explanation: http://www.atdot.net/~ko1/activities/SapporoRubyKaigi2012_ko1_pub.pdf,
     *  implementation: https://github.com/ruby/ruby/commit/b3b5e626ad69bf22be3228f847f94e1b68f40888) */
    static const int v_float_mask = 0b11;
    static const int v_float_flag = 0b10;
    inline Value _bitwise_rotate_left(Value v, int by) { return (((v) << (by)) | ((v) >> ((64 - by)))); }
    inline Value _bitwise_rotate_right(Value v, int by) { return (((v) >> (by)) | ((v) << ((64 - by)))); }
    inline Value v_from_float(double f) {
        if (f == 0.0) return 0b010;
        union { double f; Value v; } conversion;
        conversion.f = f;
        return (_bitwise_rotate_left(conversion.v, 3) & ~0b1) | v_float_flag;
    }
    inline double v_to_float(Value v) {
        if (v == 0b010) return 0.0;
        union { double f; Value v; } conversion;
        Value sign_bit = (v >> 63);
        conversion.v =
            _bitwise_rotate_right(((sign_bit ^ 1) << 1) | sign_bit | (v & ~0b11), 3);
        return conversion.f;
    }
    inline bool v_is_float(Value v) { return (v & v_float_mask) == v_float_flag; }

    /* 0 is not a valid pointer and evaluates to false in C++ */
    static const Value False = 0b0;
    /* Other constant values can be represented using the last bit: */
    static const Value True = 0b1100;
    static const Value Nothing = 0b10100;
}

#endif

