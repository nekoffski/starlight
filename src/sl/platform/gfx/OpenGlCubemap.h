#pragma once

#include "sl/gfx/Cubemap.h"

#include "sl/core/Logger.h"

namespace sl::platform::gfx {

class OpenGlCubemap : public sl::gfx::Cubemap {
public:
    explicit OpenGlCubemap(const sl::gfx::CubemapFaces&);

    ~OpenGlCubemap() override;

    void bind() override;
    void unbind() override;

private:
    unsigned int m_cubemapId;
};

struct OpenGlCubemapFactory : sl::gfx::Cubemap::Factory {
    std::unique_ptr<sl::gfx::Cubemap> create(const sl::gfx::CubemapFaces& args) override {
        return std::make_unique<OpenGlCubemap>(args);
    }
};
}
