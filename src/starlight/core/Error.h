#pragma once

#include <starlight/core/BaseError.h>

namespace starl::core {

constexpr int ERR_CATEGORY = 1;

enum class ErrorCode : unsigned int {
    NO_ERROR = 0,
    TWO_OBSERVERS_WITH_SAME_IDENT,
    TARGET_NOT_EXIST
};

class CoreException : public err::Exception {
public:
    explicit CoreException(ErrorCode code, std::string msg = "")
        : Exception(ERR_CATEGORY, static_cast<int>(code), std::move(msg)) {
        m_whatMsg = toStr();
    }

    std::string toStr() const override {
        std::ostringstream ss;
        ss << "Core Exception: " << m_code << "/" << getGlobalCode() << '/' << m_msg;
        return ss.str();
    }
};
}
