#pragma once

#include <starlight/core/BaseError.h>

namespace starl::platform {

constexpr int ERR_CATEGORY = 2;

enum class ErrorCode : unsigned int {
    NO_ERROR = 0,
    COULD_NOT_LOAD_GRAPHICS_HANDLE,
    COULD_NOT_INIT_WINDOW_LIBRARY
};

class PlatformException : public err::Exception {
public:
    explicit PlatformException(ErrorCode code)
        : Exception(ERR_CATEGORY, static_cast<int>(code)) {
    }

    std::string toStr() const override {
    }
};
}
