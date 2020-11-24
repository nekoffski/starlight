#pragma once

#include <memory>

#include "sl/math/Vector.hpp"
#include "sl/platform/gui/GUIImpl.h"



namespace sl::gui {

class Window;

class GUIProxy {
public:
    GUIProxy(std::shared_ptr<platform::gui::GUIImpl> guiImpl)
        : m_guiImpl(std::move(guiImpl)) {
    }

    Window createWindow(std::string, math::Vec2, math::Vec2);

private:
    std::shared_ptr<platform::gui::GUIImpl> m_guiImpl;
};

}
