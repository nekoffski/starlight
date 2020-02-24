#pragma once

#include <memory>

#include <starlight/platform/Error.h>

namespace starl::platform::gpu {

class GraphicsContext {
public:
    static std::unique_ptr<GraphicsContext> create(void*);

    virtual void init() = 0;
    virtual void swapBuffers() = 0;
};

} // namespace starl::platform::gpu