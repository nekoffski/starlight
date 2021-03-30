#pragma once

#include <memory>

// ifdef use_opengl
#include "sl/platform/gpu/OpenGlDefines.h"

namespace sl::gfx {

class RenderApi {
public:
    struct Factory {
        virtual std::unique_ptr<RenderApi> create() = 0;
        virtual ~Factory() = default;
    };

    virtual ~RenderApi() = default;

    inline static std::unique_ptr<Factory> factory = nullptr;

    virtual void drawArrays(unsigned, unsigned, unsigned) = 0;
    virtual void drawElements(unsigned, unsigned, unsigned) = 0;
    virtual void clearColor(float, float, float, float) = 0;

    virtual void depthMask(bool) = 0;
    virtual void setPolygonMode(unsigned) = 0;

    virtual void setDepthFunc(unsigned) = 0;
    virtual void setBlendFunc(unsigned, unsigned) = 0;

    virtual void enable(unsigned) = 0;
    virtual void disable(unsigned) = 0;
};
}
