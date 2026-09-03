#pragma once

#include "Core.hh"

namespace sl {

enum class ErrorCode {
    noError = 0,
    fileSystemError = 1,
    invalidArgument = 2,
    rendererCommandRejected = 3,
    backendMismatch = 4,
    surfaceCreationFailed = 5,
};

}  // namespace sl
