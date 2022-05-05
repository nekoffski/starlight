#pragma once

#include "sl/gpu/Texture.h"

namespace sl::platform::gl {

class OpenGlTexture : public sl::gpu::Texture {
   public:
    explicit OpenGlTexture(sl::gpu::Image&);
    explicit OpenGlTexture(unsigned int, unsigned int, int, int);

    ~OpenGlTexture() override;

    void bind(unsigned int) override;
    void unbind() override;

    unsigned int getWidth() const override;
    unsigned int getHeight() const override;
    unsigned int getBuffer() const override;

   private:
    unsigned int m_textureId;
    unsigned int m_w;
    unsigned int m_h;

    unsigned int m_lastBoundIndex = 0u;
};

struct OpenGlTextureFactory : sl::gpu::Texture::Factory {
    std::unique_ptr<sl::gpu::Texture> create(sl::gpu::Image& image) override {
        return std::make_unique<OpenGlTexture>(image);
    }

    std::unique_ptr<sl::gpu::Texture> create(
        unsigned int w, unsigned int h, int internalFormat, int format
    ) override {
        return std::make_unique<OpenGlTexture>(w, h, internalFormat, format);
    }
};
}  // namespace sl::platform::gl
