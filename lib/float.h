#ifndef LIB_FLOAT
#define LIB_FLOAT

#include "../core/oop.hpp"

using Core::Value;

namespace Lib {
    class Float : public Core::Class {
        public:
            Float(Class* super) : Core::Class(super) {
                add_method("+", 1, new Core::NativeMethod(&addition));
                add_method("-", 1, new Core::NativeMethod(&subtraction));
                add_method("*", 1, new Core::NativeMethod(&multiplication));
                add_method("/", 1, new Core::NativeMethod(&division));
            };
            static Value addition(Value inst, Value addend);
            static Value subtraction(Value inst, Value subtrahend);
            static Value multiplication(Value inst, Value factor);
            static Value division(Value inst, Value divisor);
    };
}

#endif

