#pragma once

#include "sl/gfx/FrameBuffer.h"
#include "sl/gfx/Texture.h"

namespace sl::platform::gl {

class OpenGlFrameBuffer : public sl::gfx::FrameBuffer {
   public:
    explicit OpenGlFrameBuffer();
    ~OpenGlFrameBuffer() override;

    void bind() override;
    void unbind() override;

    void bindRenderBuffer(sl::gfx::RenderBuffer&) override;
    void bindTexture(sl::gfx::Texture&, unsigned int attachment) override;
    void bindTexture(sl::gfx::Texture&) override;
    void bindCubemap(sl::gfx::Cubemap&) override;

    void specifyColorBuffers(const std::vector<unsigned int>&) override;

   private:
    unsigned int m_bufferId;
};

struct OpenGlFrameBufferFactory : sl::gfx::FrameBuffer::Factory {
    std::shared_ptr<sl::gfx::FrameBuffer> create() override {
        return std::make_shared<OpenGlFrameBuffer>();
    }
};
}  // namespace sl::platform::gl
