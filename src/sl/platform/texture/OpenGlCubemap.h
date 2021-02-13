#pragma once

#include "sl/graphics/Cubemap.h"

#include "sl/core/Logger.h"

namespace sl::platform::texture {

class OpenGlCubemap : public graphics::Cubemap {
public:
    explicit OpenGlCubemap(const graphics::CubemapArgs&);

    ~OpenGlCubemap() override;

    void bind() override;
    void unbind() override;

    graphics::CubemapArgs getFaces() override {
        return m_faces;
    }

private:
    graphics::CubemapArgs m_faces;
    unsigned int m_cubemapId;
};

struct OpenGlCubemapFactory : graphics::Cubemap::Factory {
    std::shared_ptr<graphics::Cubemap> create(const graphics::CubemapArgs& args) override {
        return std::make_shared<OpenGlCubemap>(args);
    }
};
}
