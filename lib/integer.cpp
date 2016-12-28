#include "integer.h"

using namespace Lib;
using Core::v_is_int;
using Core::v_to_int;
using Core::v_from_int;
using Core::v_is_float;
using Core::v_to_float;
using Core::v_from_float;

Value Integer::addition(Value inst, Value addend) {
    if (v_is_int(addend)) {
        return v_from_int(v_to_int(inst) + v_to_int(addend));
    } else if (v_is_float(addend)) {
        return v_from_float((double)(v_to_int(inst) + v_to_float(addend)));
    }
}

Value Integer::subtraction(Value inst, Value subtrahend) {
    if (v_is_int(subtrahend)) {
        return v_from_int(v_to_int(inst) - v_to_int(subtrahend));
    } else if (v_is_float(subtrahend)) {
        return v_from_float((double)(v_to_int(inst) - v_to_float(subtrahend)));
    }
}

Value Integer::multiplication(Value inst, Value factor) {
    if (v_is_int(factor)) {
        return v_from_int(v_to_int(inst) * v_to_int(factor));
    } else if (v_is_float(factor)) {
        return v_from_float((double)(v_to_int(inst) * v_to_float(factor)));
    }
}

Value Integer::division(Value inst, Value divisor) {
    if (v_is_int(divisor)) {
        if (v_to_int(divisor) == 0) return 0;
        auto i_dividend = v_to_int(inst);
        auto i_divisor = v_to_int(divisor);
        if ((i_dividend % i_divisor) == 0) return v_from_int(i_dividend / i_divisor);
        else return v_from_float(i_dividend / (double)(i_divisor));
    } else if (v_is_float(divisor)) {
        if (v_to_float(divisor) == 0) return 0;
        return v_from_float(v_to_int(inst) / v_to_float(divisor));
    }
}

