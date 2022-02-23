#pragma once

#include "sl/core/Input.h"

namespace sl::platform::glfw {

class GlfwInput : public core::Input {
   public:
    void init(void*) override;

    bool isKeyPressed(int) const override;

    bool isMouseButtonPressed(int) const override;

    math::Vec2 getMousePosition() const override;

    void setOnMousePositionChange(sl::core::Mouse::Callback) override;
    void setOnMouseScroll(sl::core::Mouse::Callback) override;

   private:
    void* m_windowHandle;
};

}  // namespace sl::platform::glfw