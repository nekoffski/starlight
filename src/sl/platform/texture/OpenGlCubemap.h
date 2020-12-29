#pragma once

#include "sl/graphics/Cubemap.h"

#include "sl/core/Logger.h"

namespace sl::platform::texture {

class OpenGlCubemap : public graphics::Cubemap {
public:
    explicit OpenGlCubemap(const graphics::CubemapArgs);

    ~OpenGlCubemap() override;

    void bind() override;
    void unbind() override;

private:
    unsigned int m_cubemapId;
};

struct OpenGlCubemapFactory : graphics::Cubemap::Factory {
    std::shared_ptr<graphics::Cubemap> create(const graphics::CubemapArgs& args) override {
		SL_INFO("XD??");
		return std::make_shared<OpenGlCubemap>(args);
    }
};
}
