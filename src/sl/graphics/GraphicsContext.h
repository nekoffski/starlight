#pragma once

#include <memory>

#include "sl/misc/types/NotNullPtr.hpp"
#include "sl/platform/Error.h"

namespace sl::graphics {

class GraphicsContext {
public:
    struct Factory {
        virtual std::unique_ptr<GraphicsContext> create(misc::types::NotNullPtr<void>) = 0;
    };

    inline static std::unique_ptr<Factory> factory = nullptr;

    virtual void init() = 0;
    virtual void swapBuffers() = 0;
    virtual void setViewport(unsigned, unsigned) = 0;
    virtual void clearBuffers(unsigned) = 0;
};

} // namespace sl::graphics
