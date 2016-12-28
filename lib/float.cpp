#include "float.h"

using namespace Lib;
using Core::v_is_int;
using Core::v_to_int;
using Core::v_from_int;
using Core::v_is_float;
using Core::v_to_float;
using Core::v_from_float;

Value Float::addition(Value inst, Value addend) {
    if (v_is_int(addend)) {
        return v_from_float(v_to_float(inst) + v_to_int(addend));
    } else if (v_is_float(addend)) {
        return v_from_float(v_to_float(inst) + v_to_float(addend));
    }
}

Value Float::subtraction(Value inst, Value subtrahend) {
    if (v_is_int(subtrahend)) {
        return v_from_float(v_to_float(inst) - v_to_int(subtrahend));
    } else if (v_is_float(subtrahend)) {
        return v_from_float(v_to_float(inst) - v_to_float(subtrahend));
    }
}

Value Float::multiplication(Value inst, Value factor) {
    if (v_is_int(factor)) {
        return v_from_float(v_to_float(inst) * v_to_int(factor));
    } else if (v_is_float(factor)) {
        return v_from_float(v_to_float(inst) * v_to_float(factor));
    }
}

Value Float::division(Value inst, Value divisor) {
    if (v_is_int(divisor)) {
        if (v_to_int(divisor) == 0) return 0;
        return v_from_float(v_to_float(inst) / v_to_int(divisor));
    } else if (v_is_float(divisor)) {
        if (v_to_float(divisor) == 0) return 0;
        return v_from_float(v_to_float(inst) / v_to_float(divisor));
    }
}

