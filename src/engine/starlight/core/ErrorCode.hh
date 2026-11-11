#pragma once

#include <system_error>
#include <type_traits>

namespace sl {

enum class ErrorCode {
    rendererCommandRejected = 3,
    backendMismatch = 4,
    surfaceCreationFailed = 5,
    tooManyFrameRequests = 6,
    tooManyFramesInFlight = 7,
    renderEncodingFailed = 8,
    renderSurfaceNotDrawable = 9,
    shaderParsingError = 12,
    invalidFuture = 13,
    metalError = 14,
    deviceOperationFailed = 15,
    assetNotReady = 16,
    assetBusy = 17,
    assetInUse = 18,
    notImplemented = 1337,
};

const std::error_category& errorCategory() noexcept;
std::error_code make_error_code(ErrorCode code) noexcept;

}  // namespace sl

template <>
struct std::is_error_code_enum<sl::ErrorCode> : std::true_type {};
