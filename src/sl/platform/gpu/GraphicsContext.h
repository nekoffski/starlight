#pragma once

#include <memory>

#include "sl/misc/types/NotNullPtr.hpp"
#include "sl/platform/Error.h"

namespace sl::platform::gpu {

class GraphicsContext {
public:
    static std::unique_ptr<GraphicsContext> create(misc::types::NotNullPtr<void>);

    virtual void init() = 0;
    virtual void swapBuffers() = 0;
    virtual void setViewport(unsigned, unsigned) = 0;
    virtual void clearBuffers(unsigned) = 0;
};

} // namespace sl::platform::gpu