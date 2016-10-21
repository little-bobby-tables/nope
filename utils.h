#include <sstream>
#include <iostream>

#ifndef UTILS_H
#define UTILS_H

template <typename T>
std::string to_str (T src) {
    std::ostringstream ss;
    ss << src;
    std::cout << src << " conv to " << ss.str() << std::endl;
    return ss.str();
}

#endif

