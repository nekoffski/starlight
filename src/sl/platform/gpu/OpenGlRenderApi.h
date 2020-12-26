#pragma once

#include "sl/graphics/RenderApi.h"

namespace sl::platform::gpu {

class OpenGlRenderApi : public graphics::RenderApi {
public:
    void drawArrays(unsigned, unsigned, unsigned) override;
    void drawElements(unsigned, unsigned, unsigned) override;

    void clearColor(float, float, float, float) override;

    void enableDepthMask() override;
    void disableDepthMask() override;

    void setDepthFunc(unsigned) override;
    void setBlendFunc(unsigned, unsigned) override;

    void enable(unsigned) override;
    void disable(unsigned) override;
};

struct OpenGlRenderApiFactory : graphics::RenderApi::Factory {
	std::unique_ptr<graphics::RenderApi> create() override {
		return std::make_unique<OpenGlRenderApi>();
	}
};

}
