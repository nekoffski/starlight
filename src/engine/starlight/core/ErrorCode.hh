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
    tooManyFrameRequests = 6,
    tooManyFramesInFlight = 7,
    renderEncodingFailed = 8,
    renderSurfaceNotDrawable = 9,
    jsonParsingError = 10,
    fileNotFound = 11,
};

}  // namespace sl
