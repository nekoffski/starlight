#pragma once

#include <exception>
#include <memory>
#include <string>

#include "sl/core/Logger.h"

namespace sl::core::utils {

template <typename... Args>
std::string format(const std::string& format, Args... args) {
    int size = snprintf(nullptr, 0, format.c_str(), args...) + 1;

    SL_ASSERT(size >= 0, "Fatal error while string formatting");

    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format.c_str(), args...);
    return std::string(buf.get(), buf.get() + size - 1);
}

bool isStringEmpty(const std::string& s);
}
