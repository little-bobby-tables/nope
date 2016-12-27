#include "integer.h"

using namespace Lib;
using Core::v_is_int;
using Core::v_to_int;
using Core::v_from_int;
using Core::v_is_int;
using Core::v_to_int;
using Core::v_from_int;

Value Integer::addition(Value inst, Value addend) {
    auto sum = v_to_int(inst) + v_to_int(addend);
    return v_from_int(sum);
}

Value Integer::subtraction(Value inst, Value subtrahend) {
    auto difference = v_to_int(inst) - v_to_int(subtrahend);
    return v_from_int(difference);
}

Value Integer::multiplication(Value inst, Value factor) {
    auto product = v_to_int(inst) * v_to_int(factor);
    return v_from_int(product);
}

Value Integer::division(Value inst, Value divisor) {
    if (v_to_int(divisor) == 0) {
        /* TODO: throw some sort of exception? */
        std::cout << "Division by zero is not allowed." << std::endl;
        return 0;
    }
    auto quotient = v_to_int(inst) / v_to_int(divisor);
    return v_from_int(quotient);
}

