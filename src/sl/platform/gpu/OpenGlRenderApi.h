#pragma once

#include "sl/gfx/RenderApi.h"

namespace sl::platform::gpu {

class OpenGlRenderApi : public gfx::RenderApi {
public:
    void drawArrays(unsigned, unsigned, unsigned) override;
    void drawElements(unsigned, unsigned, unsigned) override;

    void clearColor(float, float, float, float) override;

	void depthMask(bool) override;
	void setPolygonMode(unsigned) override;

    void setDepthFunc(unsigned) override;
    void setBlendFunc(unsigned, unsigned) override;

    void enable(unsigned) override;
    void disable(unsigned) override;
};

struct OpenGlRenderApiFactory : gfx::RenderApi::Factory {
    std::unique_ptr<gfx::RenderApi> create() override {
        return std::make_unique<OpenGlRenderApi>();
    }
};
}
