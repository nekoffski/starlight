#pragma once

#include <memory>

// ifdef use_opengl
#include "ViewFrustum.h"
#include "sl/glob/RendererInfo.h"
#include "sl/platform/gl/OpenGlDefines.h"

namespace sl::gfx {

class RenderApi {
   public:
    virtual ~RenderApi() = default;

    virtual void init() = 0;
    virtual void setViewport(const Viewport&) = 0;
    virtual void clearBuffers(unsigned int buffers) = 0;

    virtual void setCullFace(unsigned int) = 0;

    virtual void drawArrays(unsigned, unsigned, unsigned) = 0;
    virtual void drawElements(unsigned, unsigned, unsigned) = 0;
    virtual void clearColor(float, float, float, float) = 0;

    virtual void depthMask(bool) = 0;
    virtual void setPolygonMode(unsigned) = 0;

    virtual void setDepthFunc(unsigned) = 0;
    virtual void setBlendFunc(unsigned, unsigned) = 0;

    virtual void enable(unsigned) = 0;
    virtual void disable(unsigned) = 0;

    virtual glob::RendererInfo getRendererInfo() const = 0;
};
}  // namespace sl::gfx
