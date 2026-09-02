#pragma once

#include "Core.hh"

namespace sl {

enum class ErrorCode {
    noError = 0,
    fileSystemError = 1,
    invalidArgument = 2,
    rendererCommandRejected = 3,
};

}  // namespace sl
