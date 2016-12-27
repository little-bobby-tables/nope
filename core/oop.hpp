#ifndef CORE_CLASS
#define CORE_CLASS

#include <memory>
#include "data.hpp"
#include "../vm/instruction_sequence.hpp"
#include "../vendor/sparsepp/sparsepp.h"

using spp::sparse_hash_map;

namespace Core {
    class Class;
    class Object {
        public:
            Object() = default;
            Object(Class* c) { of_class = c; }
            Class* of_class;
    };

    typedef Value (*native_method_no_args)(Value); /* (inst) -> return value */
    typedef Value (*native_method_one_arg)(Value, Value); /* (inst, arg) -> return value */
    typedef Value (*native_method_with_args)(Value, std::vector<Value>); /* (inst, args) -> return value */

    class Method {
        public:
            bool is_native = false;
            Method() = default;
    };

    class NativeMethod : public Method {
        public:
            NativeMethod()                               { is_native = true; }
            NativeMethod(native_method_no_args method)   { code.native_0 = method; is_native = true; }
            NativeMethod(native_method_one_arg method)   { code.native_1 = method; is_native = true; }
            NativeMethod(native_method_with_args method) { code.native_many = method; is_native = true; }
            Value invoke(Value inst)                          { return code.native_0(inst); }
            Value invoke(Value inst, Value arg)               { return code.native_1(inst, arg); }
            Value invoke(Value inst, std::vector<Value> args) { return code.native_many(inst, args); }
        private:
            union {
                native_method_no_args native_0;
                native_method_one_arg native_1;
                native_method_with_args native_many;
            } code;
    };

    typedef sparse_hash_map<std::string, std::unique_ptr<Method>> MethodMap;

    class Class : public Object {
        public:
            Class() = default;
            Class(Class* super) { this->superclass = super; }
            void add_method(std::string name, int argc, Method* method) {
                std::string name_with_argc(name + std::to_string(argc));
                methods[name_with_argc] = std::unique_ptr<Method>(method);
            }
            Method* get_method(std::string name, int argc) {
                std::string name_with_argc(name + std::to_string(argc));
                MethodMap::iterator it = methods.find(name_with_argc);
                if (it != methods.end()) return it->second.get();
                else if (superclass)     return superclass->get_method(name, argc);
                else                     return nullptr;
            };
        private:
            Class* superclass;
            MethodMap methods;
    };
}

#endif

