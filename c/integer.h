#include "../data.h"

#ifndef C_INTEGER
#define C_INTEGER

CPrototype integer_proto();

CValue add(CValue left, CValue right);
CValue subtract(CValue left, CValue right);
CValue multiply(CValue left, CValue right);
CValue divide(CValue left, CValue right);

#endif
