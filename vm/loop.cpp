#include "loop.h"

using namespace VM;
using namespace Core;

void Loop::run() {
    unsigned char i;
    while (i = iseq.next()) {
        switch (i) {
            case i_put_int:
                val_stack.push(int_to_value(iseq.get_int_value()));
                break;
            case i_put_float:
                val_stack.push(float_to_value(iseq.get_float_value()));
                break;
            case i_send_msg_to_obj: {
                    std::string method = iseq.get_str_value();
                    Value arg = val_stack.top(); val_stack.pop();
                    Value obj = val_stack.top(); val_stack.pop();
                    if (is_v_int(arg) && is_v_int(obj)) {
                        if (method == "+") val_stack.push(int_to_value(value_to_int(obj) + value_to_int(arg)));
                        if (method == "-") val_stack.push(int_to_value(value_to_int(obj) - value_to_int(arg)));
                        if (method == "*") val_stack.push(int_to_value(value_to_int(obj) * value_to_int(arg)));
                        if (method == "/") val_stack.push(int_to_value(value_to_int(obj) / value_to_int(arg)));
                    }
                    if (is_v_float(arg) && is_v_float(obj)) {
                        if (method == "+") val_stack.push(float_to_value(value_to_float(obj) + value_to_float(arg)));
                        if (method == "-") val_stack.push(float_to_value(value_to_float(obj) - value_to_float(arg)));
                        if (method == "*") val_stack.push(float_to_value(value_to_float(obj) * value_to_float(arg)));
                        if (method == "/") val_stack.push(float_to_value(value_to_float(obj) / value_to_float(arg)));
                    }
                }
                break;
        }
    }
    if (is_v_int(val_stack.top())) std::cout << "=> " << value_to_int(val_stack.top()) << std::endl;
    if (is_v_float(val_stack.top())) std::cout << "=> " << value_to_float(val_stack.top()) << std::endl;
}

