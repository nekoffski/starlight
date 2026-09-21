#pragma once

#include "Metal.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/Error.hh"
#include "starlight/renderer/rhi/DeviceBuffer.hh"

namespace sl {

class MetalBuffer : public NonCopyable, public NonMovable {
    struct Guard {};

   public:
    explicit MetalBuffer(MTL::Buffer* buffer, Guard);
    ~MetalBuffer();

    static Result<std::unique_ptr<MetalBuffer>> create(
        MetalContext& ctx, const DeviceBufferDescription& description
    );

    MTL::Buffer& handle() { return *m_buffer; }

   private:
    MTL::Buffer* m_buffer;
};

}  // namespace sl
