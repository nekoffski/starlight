#pragma once

#include <starlight/core/BaseError.h>

namespace starl::platform {

constexpr int ERR_CATEGORY = 2;

enum class ErrorCode : unsigned int {
    NO_ERROR = 0,
    COULD_NOT_LOAD_GRAPHICS_HANDLE,
    COULD_NOT_INIT_WINDOW_LIBRARY,
    COULD_NOT_CREATE_WINDOW_INSTANCE
};

class PlatformException : public err::Exception {
public:
    explicit PlatformException(ErrorCode code, int thirdPartyErrorCode = 1)
        : Exception(ERR_CATEGORY, static_cast<int>(code))
        , m_thirdPartyErrorCode(thirdPartyErrorCode) {
    }

    int getThirdPartyError() const {
        return m_thirdPartyErrorCode;
    }

    std::string toStr() const override {
        std::ostringstream ss;
        ss << "[Platform Exception: " << getLocalCode() << '/' << getGlobalCode() << '/' << m_thirdPartyErrorCode << ']';
        return ss.str();
    }

private:
    int m_thirdPartyErrorCode;
};
}
