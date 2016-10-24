#include <sstream>
#include <iostream>

#ifndef UTILS_H
#define UTILS_H

template <typename T>
std::string to_str (T src) {
    std::ostringstream ss;
    ss << src;
    return ss.str();
}

#endif

