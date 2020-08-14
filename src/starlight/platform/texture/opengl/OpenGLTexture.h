#pragma once

#include "starlight/platform/texture/Texture.h"

namespace sl::platform::texture::opengl {

class OpenGLTexture : public Texture {
public:
    OpenGLTexture(const std::string&);
    ~OpenGLTexture() override;

    void bind(unsigned int) override;
    void unbind() override;

    std::shared_ptr<img::Image> getImage() {
        return m_textureImage;
    }

private:
    unsigned int m_textureId;
    std::shared_ptr<img::Image> m_textureImage;
};
}