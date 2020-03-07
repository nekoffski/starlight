#pragma once

#include <memory>

#include <starlight/core/types/NotNullPtr.hpp>
#include <starlight/platform/Error.h>

namespace starl::platform::gpu {

class GraphicsContext {
public:
    static std::unique_ptr<GraphicsContext> create(core::types::NotNullPtr<void>);

    virtual void init() = 0;
    virtual void swapBuffers() = 0;
};

} // namespace starl::platform::gpu