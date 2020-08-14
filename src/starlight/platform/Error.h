#pragma once

#include "PlatformDetector.h"
#include "starlight/core/BaseError.h"

namespace sl::platform {

constexpr int ERR_CATEGORY = 2;

enum class ErrorCode : unsigned int {
    NO_ERROR = 0,
    COULD_NOT_LOAD_GRAPHICS_HANDLE,
    COULD_NOT_INIT_WINDOW_LIBRARY,
    COULD_NOT_CREATE_WINDOW_INSTANCE,
    COULD_NOT_OPEN_FRAGMENT_SHADER,
    COULD_NOT_OPEN_VERTEX_SHADER,
    COULD_NOT_COMPILE_FRAGMENT_SHADER,
    COULD_NOT_COMPILE_VERTEX_SHADER,
    COULD_NOT_LINK_SHADER,
    COULD_NOT_LOAD_IMAGE,
    UNKNOWN_TEXTURE_FORMAT,
    COULD_NOT_LOAD_MODEL
};

class PlatformException : public err::Exception {
public:
    explicit PlatformException(ErrorCode code, std::string msg = "", int thirdPartyErrorCode = 1)
        : Exception(ERR_CATEGORY, static_cast<int>(code), std::move(msg))
        , m_thirdPartyErrorCode(thirdPartyErrorCode) {
        m_whatMsg = toStr();
    }

    int getThirdPartyError() const {
        return m_thirdPartyErrorCode;
    }

    std::string toStr() const override {
        std::ostringstream ss;
        ss << "Platform Exception: " << STARL_GET_PLATFORM_NAME() << "/" << m_code << '/' << getGlobalCode()
           << '/' << m_thirdPartyErrorCode << '/' << m_msg;
        return ss.str();
    }

private:
    int m_thirdPartyErrorCode;
};
}
