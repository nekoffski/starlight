#pragma once

#include "nova/core/Window.h"

namespace nova::platform::glfw {

class GlfwWindow : public core::Window {
   public:
    explicit GlfwWindow();

    std::string_view getVendor() const override;
    math::Size2i getSize() const override;

    void update() override;
    void swapBuffers() override;

   private:
    void* m_windowHandle;
};

}  // namespace nova::platform::glfw