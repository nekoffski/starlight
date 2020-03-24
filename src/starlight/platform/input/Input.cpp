#include <starlight/platform/input/Input.h>

#include <starlight/platform/input/glfw/GLFWInput.h>

#include <starlight/core/log/Logger.h>

static auto logger = starl::core::log::createLogger("Input");

namespace starl::platform::input {

std::unique_ptr<Input> Input::create(misc::types::NotNullPtr<void> windowHandle) {
#ifdef STARL_USE_GLFW
    logger->info("instancing glfw input");
    return std::make_unique<glfw::GLFWInput>(windowHandle);
#endif
}
} // namespace starl::platform::input
