#pragma once

#include "sl/gfx/Cubemap.h"

#include "sl/core/Logger.h"

namespace sl::platform::gfx {

class OpenGlCubemap : public sl::gfx::Cubemap {
public:
    explicit OpenGlCubemap(const sl::gfx::CubemapArgs&);

    ~OpenGlCubemap() override;

    void bind() override;
    void unbind() override;

    sl::gfx::CubemapArgs getFaces() override {
        return m_faces;
    }

private:
    sl::gfx::CubemapArgs m_faces;
    unsigned int m_cubemapId;
};

struct OpenGlCubemapFactory : sl::gfx::Cubemap::Factory {
    std::shared_ptr<sl::gfx::Cubemap> create(const sl::gfx::CubemapArgs& args) override {
        return std::make_shared<OpenGlCubemap>(args);
    }
};
}
