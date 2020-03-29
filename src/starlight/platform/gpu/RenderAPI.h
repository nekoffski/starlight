#pragma once

#include <memory>

// ifdef use_opengl
#include <starlight/platform/gpu/opengl/OpenGLDefines.h>

namespace starl::platform::gpu {

class RenderAPI {
public:
    static std::unique_ptr<RenderAPI> create();

    virtual void drawArrays(unsigned, unsigned, unsigned) = 0;
    virtual void drawElements(unsigned, unsigned, unsigned) = 0;
    virtual void clearColor(float, float, float, float) = 0;

    virtual void enableDepthMask() = 0;
    virtual void disableDepthMask() = 0;

    virtual void setDepthFunc(unsigned) = 0;
    virtual void setBlendFunc(unsigned, unsigned) = 0;

    virtual void enable(unsigned) = 0;
    virtual void disable(unsigned) = 0;
};
}