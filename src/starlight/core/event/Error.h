#pragma once 

#include <starlight/core/error/Error.h>

namespace starl::core::event {

const err::Category ERR_CATEGORY = 1;

enum class ErrorCode : int {
    NO_ERROR,
    TWO_OBSERVERS_WITH_SAME_IDENT,
    TARGET_NOT_EXIST
};

[[nodiscard]] inline err::Error makeErr(ErrorCode errCode, err::Severity severity, std::string msg="") {
    return err::Error{ERR_CATEGORY, static_cast<int>(errCode), severity, msg};
}

}
