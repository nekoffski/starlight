#pragma once

#include <memory>

#include "ViewFrustum.h"
#include "sl/core/types/NotNullPtr.hpp"

namespace sl::gfx {

class GraphicsContext {
public:
    struct Factory {
        virtual std::unique_ptr<GraphicsContext> create(core::types::NotNullPtr<void>) = 0;
    };

    inline static std::unique_ptr<Factory> factory = nullptr;

    virtual void swapBuffers() = 0;
    virtual void setViewport(ViewFrustum::Viewport) = 0;
    virtual void clearBuffers(unsigned) = 0;
};

} // namespace sl::gfx
