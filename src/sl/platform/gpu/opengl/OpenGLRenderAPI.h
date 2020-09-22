#pragma once

#include "sl/platform/gpu/RenderAPI.h"

namespace sl::platform::gpu::opengl {

class OpenGLRenderAPI : public RenderAPI {
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
}