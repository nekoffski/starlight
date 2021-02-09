#include "String.hpp"

namespace sl::core::utils {

bool isStringEmpty(const std::string& s) {
    return s[0] == '\0';
}
}
