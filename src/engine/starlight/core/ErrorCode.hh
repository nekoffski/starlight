#pragma once

#include "Core.hh"

namespace starlight {

enum class ErrorCode {
    noError = 0,
    fileSystemError = 1,
    invalidArgument = 2,
};

}  // namespace starlight
