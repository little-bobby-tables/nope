#ifndef LIB_OBJECT
#define LIB_OBJECT

#include "../core/oop.hpp"

using Core::Value;

namespace Lib {
    class Object : public Core::Class {
        public:
            Object() : Core::Class(nullptr) {
                add_method("+", 1, new Core::NativeMethod(&method_missing));
            };
            static Value method_missing(Value inst, Value method);
    };
}

#endif

