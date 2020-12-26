#pragma once

#include "sl/core/Input.h"
#include "sl/misc/types/NotNullPtr.hpp"

#include <GLFW/glfw3.h>

namespace sl::platform::input {

class GlfwInput : public core::Input {
public:
    explicit GlfwInput(void*);

    void setKeyCallback(core::InputCallback) override;

    bool isKeyPressed(int) const override;
    bool isMouseButtonPressed(int) const override;
    std::pair<double, double> getMousePosition() const override;

private:
    using Input::Input;

    GLFWwindow* m_windowHandle;
};

struct GlfwInputFactory : core::Input::Factory {
	virtual std::shared_ptr<core::Input> create(misc::types::NotNullPtr<void> windowHandle) {
		return std::make_shared<GlfwInput>(windowHandle);
	}
};

}
