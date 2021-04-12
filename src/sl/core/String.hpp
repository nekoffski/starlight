#pragma once

#include <exception>
#include <memory>
#include <string>

#include "sl/core/Logger.h"

namespace sl::core {

bool isStringEmpty(const std::string& s);

inline void stripString(std::string& str) {
    str.erase(std::find(str.begin(), str.end(), '\0'), str.end());
}
}
