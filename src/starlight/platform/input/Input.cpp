#include <starlight/platform/input/Input.h>

#include <starlight/platform/input/glfw/GLFWInput.h>

namespace starl::platform::input {

std::unique_ptr<Input> Input::create(core::types::NotNullPtr<void> windowHandle) {
#ifdef STARL_USE_GLFW
    return std::make_unique<glfw::GLFWInput>(windowHandle);
#endif
}
} // namespace starl::platform::input
