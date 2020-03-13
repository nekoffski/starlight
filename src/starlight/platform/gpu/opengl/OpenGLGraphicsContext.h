// that header must be included first due to GLAD
#include <starlight/platform/gpu/GraphicsContext.h>

#include <GLFW/glfw3.h>

namespace starl::platform::gpu::opengl {

class OpenGLGraphicsContext : public GraphicsContext {
public:
    explicit OpenGLGraphicsContext(void*);

    void init() override;
    void swapBuffers() override;
    void clearBuffers() override;

private:
    GLFWwindow* m_windowHandle;
};

} // namespace starl::platform::gpu::opengl
