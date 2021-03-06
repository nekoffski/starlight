#pragma once

#include "sl/gfx/Texture.h"

namespace sl::platform::gfx {

class OpenGlTexture : public sl::gfx::Texture {
public:
    explicit OpenGlTexture(sl::gfx::Image&);
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

struct OpenGlTextureFactory : sl::gfx::Texture::Factory {
    std::unique_ptr<sl::gfx::Texture> create(sl::gfx::Image& image) override {
        return std::make_unique<OpenGlTexture>(image);
    }

    std::unique_ptr<sl::gfx::Texture> create(unsigned int w, unsigned int h, int internalFormat, int format) override {
        return std::make_unique<OpenGlTexture>(w, h, internalFormat, format);
    }
};
}
