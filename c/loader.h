#include "../data.h"
#include "integer.h"

#ifndef C_LOADER
#define C_LOADER

inline void load_protos(std::vector<CPrototype> &protos) {
    protos.push_back(integer_proto());
}

#endif
