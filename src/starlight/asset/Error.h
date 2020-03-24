#pragma once

#include <starlight/core/BaseError.h>

namespace starl::asset {

constexpr int ERR_CATEGORY = 3;

enum class ErrorCode : unsigned int {
    NO_ERROR = 0,
    PATH_ALREADY_REGISTERED,
    PATH_NOT_REGISTERED,
    LOADER_NOT_FOUND
};

class AssetException : public err::Exception {
public:
    explicit AssetException(ErrorCode code, std::string msg = "")
        : Exception(ERR_CATEGORY, static_cast<int>(code), std::move(msg)) {
        m_whatMsg = toStr();
    }

    std::string toStr() const override {
        std::ostringstream ss;
        ss << "Asset Exception: " << m_code << "/" << getGlobalCode() << '/' << m_msg;
        return ss.str();
    }
};
}
