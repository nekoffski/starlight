#pragma once

#include <starlight/platform/gpu/RenderAPI.h>

namespace starl::platform::gpu::opengl {

class OpenGLRenderAPI : public RenderAPI {
public:
    void drawArrays(unsigned, unsigned, unsigned) override;
    void clearColor(float, float, float, float) override;
};
}