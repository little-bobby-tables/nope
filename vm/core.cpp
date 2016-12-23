#include "core.h"

using namespace VM;

void Core::loop() {
    unsigned char i;
    while (i = iseq.next()) {
        switch (i) {
            case i_put_int: {
                    long val = iseq.get_int_value();
                    Value v = int_to_value(val);
                    val_stack.push(v);
                }
                break;
            case i_send_msg_to_obj: {
                    std::string method = iseq.get_str_value();
                    if (method == "+") {
                        long arg = value_to_int(val_stack.top()); val_stack.pop();
                        long obj = value_to_int(val_stack.top()); val_stack.pop();
                        std::cout << "=> " << obj << " + " << arg << " = " << arg + obj << std::endl;
                    }
                }
                break;
        }
    }
}

