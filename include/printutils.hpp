#pragma once
#include <iostream>
namespace io_utils {
    inline void print() {}
    template<typename T, typename... Args>
    inline void print(const T& first, const Args... rest_args) {
        std::cout << first;
        print(rest_args...);
    }
}
