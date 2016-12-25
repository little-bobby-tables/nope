#ifndef CORE_CLASS
#define CORE_CLASS

#include "data.hpp"

namespace Core {
    struct Method {
        union {
            /* return type <- (self, arguments) */
            Value (*native_impl)(Value, std::vector<Value>);
            Value dynamic_impl;
        } code;
        bool is_native;
    };

    class Class {
        public:
            Value superclass;
    };
}



#endif

