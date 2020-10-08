#pragma once

#include "sl/platform/texture/Texture.h"

namespace sl::platform::texture::opengl {

class OpenGLTexture : public Texture {
public:
    explicit OpenGLTexture(const std::string&);
    explicit OpenGLTexture(unsigned int, unsigned int);
    ~OpenGLTexture() override;

    void bind(unsigned int) override;
    void unbind() override;

    unsigned int getWidth() const override {
        return m_w;
    }

    unsigned int getHeight() const override {
        return m_h;
    }

    std::shared_ptr<img::Image> getImage() {
        return m_textureImage;
    }

    unsigned int getBuffer() const override {
        return m_textureId;
    }

private:
    unsigned int m_textureId;
    std::shared_ptr<img::Image> m_textureImage;

    unsigned int m_w;
    unsigned int m_h;
};
}