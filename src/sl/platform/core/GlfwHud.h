#pragma once

#include "sl/core/Hud.h"
#include "sl/core/NotNullPtr.hpp"

namespace sl::platform::core {

class GlfwHud : public sl::core::Hud {
public:
    explicit GlfwHud(void* windowHandle);

    bool isKeyPressed(int) const override;

    bool isMouseButtonPressed(int) const override;

    std::pair<double, double> getMousePosition() const override;

    void setOnMousePositionChange(sl::core::Mouse::Callback) override;
    void setOnMouseScroll(sl::core::Mouse::Callback) override;

private:
    void* m_windowHandle;
};

}