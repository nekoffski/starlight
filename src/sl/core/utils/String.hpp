#pragma once

#include <exception>
#include <memory>
#include <string>

namespace sl::core::utils {

template <typename... Args>
std::string format(const std::string& format, Args... args) {
    int size = snprintf(nullptr, 0, format.c_str(), args...) + 1;

    if (size <= 0)
        throw std::runtime_error("Error during formatting.");

    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format.c_str(), args...);
    return std::string(buf.get(), buf.get() + size - 1);
}

inline bool isStringEmpty(const std::string& s) {
    return s[0] == '\0';
}
}
