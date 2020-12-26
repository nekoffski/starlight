#pragma once

#include <memory>

// ifdef use_opengl
#include "sl/platform/gpu/OpenGlDefines.h"

namespace sl::graphics {

class RenderApi {
public:
    struct Factory {
        virtual std::unique_ptr<RenderApi> create() = 0;
    };

    inline static std::unique_ptr<Factory> factory = nullptr;

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
