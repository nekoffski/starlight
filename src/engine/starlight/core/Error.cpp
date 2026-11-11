#include "Error.hh"

namespace sl {
namespace {

class ErrorCategory final : public std::error_category {
   public:
    const char* name() const noexcept override { return "starlight"; }

    std::string message(int value) const override {
        switch (static_cast<ErrorCode>(value)) {
            case ErrorCode::rendererCommandRejected:
                return "Renderer command rejected";
            case ErrorCode::backendMismatch:
                return "Backend mismatch";
            case ErrorCode::surfaceCreationFailed:
                return "Surface creation failed";
            case ErrorCode::tooManyFrameRequests:
                return "Too many frame requests";
            case ErrorCode::tooManyFramesInFlight:
                return "Too many frames in flight";
            case ErrorCode::renderEncodingFailed:
                return "Render encoding failed";
            case ErrorCode::renderSurfaceNotDrawable:
                return "Render surface not drawable";
            case ErrorCode::shaderParsingError:
                return "Shader parsing error";
            case ErrorCode::invalidFuture:
                return "Invalid future";
            case ErrorCode::metalError:
                return "Metal error";
            case ErrorCode::deviceOperationFailed:
                return "Device operation failed";
            case ErrorCode::assetNotReady:
                return "Asset not ready";
            case ErrorCode::assetBusy:
                return "Asset operation pending";
            case ErrorCode::assetInUse:
                return "Asset still referenced";
            case ErrorCode::notImplemented:
                return "Not implemented";
        }
        return "Unknown Starlight error";
    }
};

}  // namespace

const std::error_category& errorCategory() noexcept {
    static ErrorCategory category;
    return category;
}

std::error_code make_error_code(ErrorCode code) noexcept {
    return {static_cast<int>(code), errorCategory()};
}

}  // namespace sl
