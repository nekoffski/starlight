#pragma once

#include "sl/gfx/Cubemap.h"

#include "sl/core/Logger.h"

namespace sl::platform::texture {

class OpenGlCubemap : public gfx::Cubemap {
public:
    explicit OpenGlCubemap(const gfx::CubemapArgs&);

    ~OpenGlCubemap() override;

    void bind() override;
    void unbind() override;

    gfx::CubemapArgs getFaces() override {
        return m_faces;
    }

private:
    gfx::CubemapArgs m_faces;
    unsigned int m_cubemapId;
};

struct OpenGlCubemapFactory : gfx::Cubemap::Factory {
    std::shared_ptr<gfx::Cubemap> create(const gfx::CubemapArgs& args) override {
        return std::make_shared<OpenGlCubemap>(args);
    }
};
}
