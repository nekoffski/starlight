#pragma once

#include <memory>

#include "ViewFrustum.h"
#include "sl/core/NotNullPtr.hpp"

namespace sl::gfx {

class GraphicsContext {
public:
    struct Factory {
        virtual std::shared_ptr<GraphicsContext> create(core::NotNullPtr<void>) = 0;
        virtual ~Factory() = default;
    };

    virtual ~GraphicsContext() = default;

    inline static std::unique_ptr<Factory> factory = nullptr;

    virtual void swapBuffers() = 0;
    virtual void setViewport(ViewFrustum::Viewport) = 0;
    virtual void clearBuffers(unsigned) = 0;
};

} // namespace sl::gfx
