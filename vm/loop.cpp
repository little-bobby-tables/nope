#include "loop.h"

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
                    auto arg = v_stack.top(); v_stack.pop();
                    auto obj = v_stack.top(); v_stack.pop();
                    auto result = dispatcher->invoke_method(obj, method, arg);
                    v_stack.push(result);
                }
                break;
        }
    }
    if (v_is_int(v_stack.top())) std::cout << "=> " << v_to_int(v_stack.top()) << std::endl;
    if (v_is_float(v_stack.top())) std::cout << "=> " << v_to_float(v_stack.top()) << std::endl;
}

