#pragma once

#include <memory>

#include "sl/core/types/NotNullPtr.hpp"

namespace sl::graphics {

class GraphicsContext {
public:
    struct Factory {
        virtual std::unique_ptr<GraphicsContext> create(core::types::NotNullPtr<void>) = 0;
    };

    inline static std::unique_ptr<Factory> factory = nullptr;

    virtual void swapBuffers() = 0;
    virtual void setViewport(unsigned, unsigned) = 0;
    virtual void clearBuffers(unsigned) = 0;
};

} // namespace sl::graphics
