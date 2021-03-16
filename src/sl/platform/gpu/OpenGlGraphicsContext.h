// that header must be included first due to GLAD

#include "sl/core/types/NotNullPtr.hpp"
#include "sl/graphics/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace sl::platform::gpu {

class OpenGlGraphicsContext : public graphics::GraphicsContext {
public:
    explicit OpenGlGraphicsContext(void*);

    void swapBuffers() override;
    void clearBuffers(unsigned) override;
    void setViewport(graphics::ViewFrustum::Viewport) override;

private:
    GLFWwindow* m_windowHandle;
};

struct OpenGlGraphicsContextFactory : graphics::GraphicsContext::Factory {
    std::unique_ptr<graphics::GraphicsContext> create(core::types::NotNullPtr<void> windowHandle) override {
        return std::make_unique<OpenGlGraphicsContext>(windowHandle);
    }
};

} // namespace sl::graphics
