#pragma once

#include "sl/platform/input/Input.h"

#include <GLFW/glfw3.h>

namespace sl::platform::input::glfw {

class GLFWInput : public Input {
public:
    explicit GLFWInput(void*);

    void setKeyCallback(InputCallback) override;

    bool isKeyPressed(int) const override;
    bool isMouseButtonPressed(int) const override;
    std::pair<double, double> getMousePosition() const override;

private:
    using Input::Input;

    GLFWwindow* m_windowHandle;
};
}
