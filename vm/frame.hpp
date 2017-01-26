#ifndef VM_FRAME
#define VM_FRAME

#include "../core/data.hpp"
#include "../vendor/sparsepp/sparsepp.h"

using spp::sparse_hash_map;

namespace VM {
    typedef sparse_hash_map<std::string, Core::Value> VariableMap;

    class Frame {
        public:
            Frame() = default;
            Frame* parent = nullptr;

            void set_variable(std::string name, Core::Value val) {
                variables[name] = val;
            }

            Core::Value get_variable(std::string name) {
                VariableMap::iterator it = variables.find(name);
                if (it != variables.end()) return it->second;
                else if (parent) return parent->get_variable(name);
                return Core::Undefined;
            }
      
        private:
            VariableMap variables;
    };
}

#endif

