#pragma once

#include "sl/platform/gpu/FrameBuffer.h"

namespace sl::platform::gpu::opengl {

class OpenGLFrameBuffer : public FrameBuffer {
public:
    explicit OpenGLFrameBuffer();
    ~OpenGLFrameBuffer() override;

    void bind() override;
    void unbind() override;
    void bindTexture(std::shared_ptr<texture::Texture>) override;

private:
    unsigned int m_bufferId;
};
}