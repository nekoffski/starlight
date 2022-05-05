#pragma once

#include <kc/core/Log.h>

#include "sl/gpu/Cubemap.h"

namespace sl::platform::gl {

class OpenGlCubemap : public sl::gpu::Cubemap {
   public:
    explicit OpenGlCubemap(unsigned int width, unsigned int height);
    explicit OpenGlCubemap(const sl::gpu::CubemapFaces&);

    ~OpenGlCubemap() override;

    void bind(unsigned int index) override;
    void unbind() override;

    unsigned int getBufferId() const override { return m_cubemapId; }

   private:
    unsigned int m_cubemapId;
    unsigned int m_lastBoundIndex = 0u;
};

struct OpenGlCubemapFactory : sl::gpu::Cubemap::Factory {
    std::unique_ptr<sl::gpu::Cubemap> create(const sl::gpu::CubemapFaces& args) override {
        return std::make_unique<OpenGlCubemap>(args);
    }

    std::unique_ptr<sl::gpu::Cubemap> create(unsigned int width, unsigned int height) override {
        return std::make_unique<OpenGlCubemap>(width, height);
    }
};
}  // namespace sl::platform::gl
