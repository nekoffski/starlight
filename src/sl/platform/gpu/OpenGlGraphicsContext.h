// that header must be included first due to GLAD

#include "sl/core/types/NotNullPtr.hpp"
#include "sl/gfx/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace sl::platform::gpu {

class OpenGlGraphicsContext : public gfx::GraphicsContext {
public:
    explicit OpenGlGraphicsContext(void*);

    void swapBuffers() override;
    void clearBuffers(unsigned) override;
    void setViewport(gfx::ViewFrustum::Viewport) override;

private:
    GLFWwindow* m_windowHandle;
};

struct OpenGlGraphicsContextFactory : gfx::GraphicsContext::Factory {
    std::shared_ptr<gfx::GraphicsContext> create(core::types::NotNullPtr<void> windowHandle) override {
        return std::make_shared<OpenGlGraphicsContext>(windowHandle);
    }
};

} // namespace sl::gfx
