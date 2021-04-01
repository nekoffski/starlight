#pragma once

#include "sl/gfx/Texture.h"

namespace sl::platform::gfx {

class OpenGlTexture : public sl::gfx::Texture {
public:
    explicit OpenGlTexture(const std::string&);
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

    std::shared_ptr<sl::gfx::Image> getImage() {
        return m_textureImage;
    }

    unsigned int getBuffer() const override {
        return m_textureId;
    }

private:
    unsigned int m_textureId;
    std::shared_ptr<sl::gfx::Image> m_textureImage;

    unsigned int m_w;
    unsigned int m_h;
};

struct OpenGlTextureFactory : sl::gfx::Texture::Factory {
    std::shared_ptr<sl::gfx::Texture> create(const std::string& path) override {
        return std::make_shared<OpenGlTexture>(path);
    }

    std::shared_ptr<sl::gfx::Texture> create(unsigned int w, unsigned int h) override {
        return std::make_shared<OpenGlTexture>(w, h);
    }
};
}
