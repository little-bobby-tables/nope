#ifndef CORE_CLASS
#define CORE_CLASS

#include "data.hpp"
#include "../vm/instruction_sequence.hpp"
#include "../vendor/sparsepp/sparsepp.h"

using spp::sparse_hash_map;

namespace Core {
    class Object {
        public:
            Object() = default;
    };

    typedef Value (*native_method_no_args)(Value); /* (Value inst) -> Value */
    typedef Value (*native_method_one_arg)(Value, Value); /* (Value inst, Value arg1) -> Value */

    class Method {
        public:
            bool is_native = false;
            Method() = default;
    };

    class NativeMethod : public Method {
        public:
            NativeMethod() { is_native = true; }
            NativeMethod(native_method_no_args method) {
                code.native0 = method; is_native = true; argc = 0;
            }
            NativeMethod(native_method_one_arg method) {
                code.native1 = method; is_native = true; argc = 1;
            }
            Value invoke(Value inst) { code.native0(inst); }
            Value invoke(Value inst, Value arg1) { code.native1(inst, arg1); }
        private:
            union {
                native_method_no_args native0;
                native_method_one_arg native1;
            } code;
            int argc;
    };

    typedef sparse_hash_map<std::string, Method> MethodMap;

    class Class : public Object {
        public:
            Class() = default;
            Class(Class* super) { this->superclass = super; }
            void add_method(std::string name, int argc, Method method) {
                std::string name_with_argc(name + std::to_string(argc));
                methods[name_with_argc] = method;
            }
            Method get_method(std::string name, int argc) {
                std::string name_with_argc(name + std::to_string(argc));
                MethodMap::iterator it = methods.find(name_with_argc);
                if (it != methods.end()) {
                    return it->second;
                } else if (superclass) {
                    return superclass->get_method(name, argc);
                } else {
                    std::cout << "Method missing! " << name << " (argc: " << argc << ")" << std::endl;
                    return Method();
                }
            };
        private:
            Class* superclass;
            MethodMap methods;
    };
}



#endif

