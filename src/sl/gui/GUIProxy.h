#pragma once

#include <memory>

#include "sl/math/Vector.hpp"
#include "sl/platform/gui/GUIAdapter.hpp"

namespace sl::gui {

class Window;

class GUIProxy {
public:
    GUIProxy(std::shared_ptr<platform::gui::GUIAdapter> guiAdapter)
        : m_guiAdapter(std::move(guiAdapter)) {
    }

    Window createWindow(std::string, math::Vec2, math::Vec2);

private:
    std::shared_ptr<platform::gui::GUIAdapter> m_guiAdapter;
};
}