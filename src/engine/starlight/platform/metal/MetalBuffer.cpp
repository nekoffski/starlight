#include "MetalBuffer.hh"

namespace sl {

MetalBuffer::MetalBuffer(MTL::Buffer* buffer, Guard) : m_buffer(buffer) {}

MetalBuffer::~MetalBuffer() {
    if (m_buffer) {
        m_buffer->release();
    }
}

Result<std::unique_ptr<MetalBuffer>> MetalBuffer::create(
    MetalContext& ctx, const DeviceBufferDescription& description
) {
    if (description.bytes.empty()) {
        return Error::unexpected(
            ErrorCode::invalidArgument, "Buffer description is empty"
        );
    }

    if (description.bytes.size() > ctx.device().maxBufferLength()) {
        return Error::unexpected(
            ErrorCode::invalidArgument,
            "Buffer description exceeds maximum length"
        );
    }

    auto* buffer = ctx.device().newBuffer(
        description.bytes.data(),
        static_cast<NS::Integer>(description.bytes.size()),
        MTL::ResourceStorageModeShared
    );

    if (not buffer) {
        return Error::unexpected(
            ErrorCode::metalError, "Failed to create Metal buffer"
        );
    }

    return std::unique_ptr<MetalBuffer>(
        new MetalBuffer(buffer, MetalBuffer::Guard{})
    );
}

}  // namespace sl
