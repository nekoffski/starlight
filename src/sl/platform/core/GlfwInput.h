#pragma once

#include "sl/core/Input.h"
#include "sl/core/types/NotNullPtr.hpp"

#include <GLFW/glfw3.h>

namespace sl::platform::core {

class GlfwInput : public sl::core::Input {
public:
    explicit GlfwInput(void*);

    void setKeyCallback(sl::core::InputCallback) override;

    bool isKeyPressed(int) const override;
    bool isMouseButtonPressed(int) const override;

    void update() override;

    std::pair<double, double> getMousePosition() const override;
    std::pair<double, double> getMousePositonDelta() const override;
    double getScrollDelta() const override;

private:
    using Input::Input;

    GLFWwindow* m_windowHandle;
};

struct GlfwInputFactory : sl::core::Input::Factory {
    virtual std::shared_ptr<sl::core::Input> create(sl::core::types::NotNullPtr<void> windowHandle) {
        return std::make_shared<GlfwInput>(windowHandle);
    }
};
}
