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

inline std::string extractNameFromPath(const std::string& path) {
    auto nameBegin = path.find_last_of("/") + 1;
    return path.substr(
        nameBegin,
        path.find_last_of(".") - nameBegin);
}
}
