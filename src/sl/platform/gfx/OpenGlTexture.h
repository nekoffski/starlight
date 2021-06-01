#pragma once

#include "sl/gfx/Texture.h"

namespace sl::platform::gfx {

class OpenGlTexture : public sl::gfx::Texture {
public:
    explicit OpenGlTexture(sl::gfx::Image&);
    explicit OpenGlTexture(unsigned int, unsigned int);

    ~OpenGlTexture() override;

    void bind(unsigned int) override;
    void unbind() override;

    unsigned int getWidth() const override {
        return m_w;
    }

    unsigned int getHeight() const override {
        return m_h;
    }

    unsigned int getBuffer() const override {
        return m_textureId;
    }

private:
    unsigned int m_textureId;

    unsigned int m_w;
    unsigned int m_h;
};

struct OpenGlTextureFactory : sl::gfx::Texture::Factory {
    std::shared_ptr<sl::gfx::Texture> create(sl::gfx::Image& image) override {
        return std::make_shared<OpenGlTexture>(image);
    }

    std::shared_ptr<sl::gfx::Texture> create(unsigned int w, unsigned int h) override {
        return std::make_shared<OpenGlTexture>(w, h);
    }
};
}
