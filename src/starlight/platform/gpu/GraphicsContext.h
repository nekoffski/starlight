#pragma once

#include <memory>

namespace starl::platform::gpu {

class GraphicsContext {
public:
    static std::unique_ptr<GraphicsContext> create();

    virtual void init() = 0;
    virtual void swapBuffers() = 0;
};

}