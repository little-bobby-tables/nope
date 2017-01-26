#include "loop.h"

#include "frame.hpp"

using namespace VM;
using namespace Core;

void Loop::run() {
    unsigned char i;
    Frame *frame = new Frame();

    while (i = iseq.next()) {
        switch (i) {
            case i_put_int:
                v_stack.push(v_from_int(iseq.get_int_value()));
                break;
            case i_put_float:
                v_stack.push(v_from_float(iseq.get_float_value()));
                break;
            case i_send_msg_to_obj: {
                    auto method = iseq.get_str_value();
                    auto arg = v_stack.top(); v_stack.pop();
                    auto obj = v_stack.top(); v_stack.pop();
                    auto result = dispatcher->invoke_method(obj, method, arg);
                    v_stack.push(result);
                }
                break;
            case i_put_reference: {
                    auto name = iseq.get_str_value();
                    auto value = frame->get_variable(name);
                    v_stack.push(value);
                }
                break;
            case i_assign_reference: {
                    auto value = v_stack.top(); /* Don't pop the value off the stack! */
                    auto name = iseq.get_str_value();
                    frame->set_variable(name, value);
               }
               break;
        }
    }
    if (v_stack.top() == Undefined) std::cout << "undefined" << std::endl;
    if (v_is_int(v_stack.top())) std::cout << "=> " << v_to_int(v_stack.top()) << " (Integer)" << std::endl;
    if (v_is_float(v_stack.top())) std::cout << "=> " << v_to_float(v_stack.top()) << " (Float)" << std::endl;
}

