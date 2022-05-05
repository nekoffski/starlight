#pragma once

#include "sl/gpu/FrameBuffer.h"
#include "sl/gpu/Texture.h"

namespace sl::platform::gl {

class OpenGlFrameBuffer : public sl::gpu::FrameBuffer {
   public:
    explicit OpenGlFrameBuffer();
    ~OpenGlFrameBuffer() override;

    void bind() override;
    void unbind() override;

    void bindRenderBuffer(sl::gpu::RenderBuffer&) override;
    void bindTexture(sl::gpu::Texture&, unsigned int attachment) override;
    void bindTexture(sl::gpu::Texture&) override;
    void bindCubemap(sl::gpu::Cubemap&) override;

    void specifyColorBuffers(const std::vector<unsigned int>&) override;

   private:
    unsigned int m_bufferId;
};

struct OpenGlFrameBufferFactory : sl::gpu::FrameBuffer::Factory {
    std::shared_ptr<sl::gpu::FrameBuffer> create() override {
        return std::make_shared<OpenGlFrameBuffer>();
    }
};
}  // namespace sl::platform::gl
