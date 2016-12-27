#ifndef VM_DISPATCHER
#define VM_DISPATCHER

#include "../lib/object.h"
#include "../lib/integer.h"
#include "../vendor/sparsepp/sparsepp.h"

using spp::sparse_hash_map;
using namespace Core;

namespace VM {
    typedef sparse_hash_map<std::string, std::unique_ptr<Class>> ClassMap;

    class Dispatcher {
        public:
            Dispatcher() {
                classes["Object"] = std::make_unique<Lib::Object>();
                Class* object = classes["Object"].get();

                classes["Integer"] = std::make_unique<Lib::Integer>(object);
            };
            Value invoke_method(Value inst, std::string method) {
                Class* c = get_class(inst);
                Method* m = c->get_method(method, 0);
                if (m == nullptr)      return method_missing(c, inst, method,std::vector<Value>());
                else if (m->is_native) return static_cast<NativeMethod*>(m)->invoke(inst);
                else                   return 0;
            }
            Value invoke_method(Value inst, std::string method, Value arg) {
                Class* c = get_class(inst);
                Method* m = c->get_method(method, 1);
                if (m == nullptr)      return method_missing(c, inst, method, { arg });
                else if (m->is_native) return static_cast<NativeMethod*>(m)->invoke(inst, arg);
                else                   return 0;
            }
            Value invoke_method(Value inst, std::string method, std::vector<Value> args) {
                Class* c = get_class(inst);
                Method* m = c->get_method(method, args.size());
                if (m == nullptr)      return method_missing(c, inst, method, args);
                else if (m->is_native) return static_cast<NativeMethod*>(m)->invoke(inst, args);
                else                   return 0;
            }
            Value method_missing(Class* c, Value inst, std::string method, std::vector<Value> args) {
                args.insert(args.begin(), /* TODO: method -> Value ? */ 0);
                Method* m = c->get_method("method_missing", 2);
                if (m->is_native)      return static_cast<NativeMethod*>(m)->invoke(inst, args);
                else                   return 0;
            }
        private:
            Class* get_class(Value v) {
                if (v_is_int(v))   return classes["Integer"].get();
                if (v_is_float(v)) return classes["Float"].get();
                if (v == False)    return classes["FalseClass"].get();
                if (v == True)     return classes["TrueClass"].get();
                if (v == Nothing)  return classes["NothingClass"].get();
                return ((Core::Object*)v)->of_class;
            };
            ClassMap classes;
    };
}

#endif

