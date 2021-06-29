#pragma once

#include "sl/gfx/Cubemap.h"

#include "sl/core/Logger.h"

namespace sl::platform::gfx {

class OpenGlCubemap : public sl::gfx::Cubemap {
public:
    explicit OpenGlCubemap(unsigned int width, unsigned int height);
    explicit OpenGlCubemap(const sl::gfx::CubemapFaces&);

    ~OpenGlCubemap() override;

    void bind(unsigned int index) override;
    void unbind() override;

    unsigned int getBufferId() const override {
        return m_cubemapId;
    }

private:
    unsigned int m_cubemapId;
    unsigned int m_lastBoundIndex = 0u;
};

struct OpenGlCubemapFactory : sl::gfx::Cubemap::Factory {
    std::unique_ptr<sl::gfx::Cubemap> create(const sl::gfx::CubemapFaces& args) override {
        return std::make_unique<OpenGlCubemap>(args);
    }

    std::unique_ptr<sl::gfx::Cubemap> create(unsigned int width, unsigned int height) override {
        return std::make_unique<OpenGlCubemap>(width, height);
    }
};
}
