#pragma once

#include "sl/core/types/NotNullPtr.hpp"
#include "sl/gfx/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace sl::platform::gfx {

class OpenGlGraphicsContext : public sl::gfx::GraphicsContext {
public:
    explicit OpenGlGraphicsContext(void*);

    void swapBuffers() override;
    void clearBuffers(unsigned) override;
    void setViewport(sl::gfx::ViewFrustum::Viewport) override;

private:
    GLFWwindow* m_windowHandle;
};

struct OpenGlGraphicsContextFactory : sl::gfx::GraphicsContext::Factory {
    std::shared_ptr<sl::gfx::GraphicsContext> create(sl::core::types::NotNullPtr<void> windowHandle) override {
        return std::make_shared<OpenGlGraphicsContext>(windowHandle);
    }
};

} // namespace sl::gfx
