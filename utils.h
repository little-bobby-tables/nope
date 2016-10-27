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

inline int to_i(std::string s) {
    return atoi(s.c_str());
}

#endif

