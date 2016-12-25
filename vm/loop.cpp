#include "loop.h"

#include "../core/oop.hpp"

using namespace VM;
using namespace Core;

void Loop::run() {
    unsigned char i;
    while (i = iseq.next()) {
        switch (i) {
            case i_put_int:
                v_stack.push(v_from_int(iseq.get_int_value()));
                break;
            case i_put_float:
                v_stack.push(v_from_float(iseq.get_float_value()));
                break;
            case i_send_msg_to_obj: {
                    std::string method = iseq.get_str_value();
                    Value arg = v_stack.top(); v_stack.pop();
                    Value obj = v_stack.top(); v_stack.pop();
                    if (v_is_int(arg) && v_is_int(obj)) {
                        if (method == "+") v_stack.push(v_from_int(v_to_int(obj) + v_to_int(arg)));
                        if (method == "-") v_stack.push(v_from_int(v_to_int(obj) - v_to_int(arg)));
                        if (method == "*") v_stack.push(v_from_int(v_to_int(obj) * v_to_int(arg)));
                        if (method == "/") v_stack.push(v_from_int(v_to_int(obj) / v_to_int(arg)));
                    }
                    if (v_is_float(arg) && v_is_float(obj)) {
                        if (method == "+") v_stack.push(v_from_float(v_to_float(obj) + v_to_float(arg)));
                        if (method == "-") v_stack.push(v_from_float(v_to_float(obj) - v_to_float(arg)));
                        if (method == "*") v_stack.push(v_from_float(v_to_float(obj) * v_to_float(arg)));
                        if (method == "/") v_stack.push(v_from_float(v_to_float(obj) / v_to_float(arg)));
                    }
                }
                break;
        }
    }
    if (v_is_int(v_stack.top())) std::cout << "=> " << v_to_int(v_stack.top()) << std::endl;
    if (v_is_float(v_stack.top())) std::cout << "=> " << v_to_float(v_stack.top()) << std::endl;
}

