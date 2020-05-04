#pragma once

#include <memory>

#include <starlight/math/Vector.hpp>
#include <starlight/platform/gui/GUIAdapter.hpp>

namespace starl::gui {

class Window;

class GUIProxy {
public:
    GUIProxy(std::shared_ptr<platform::gui::GUIAdapter> guiAdapter)
        : m_guiAdapter(std::move(guiAdapter)) {
    }

    Window createWindow(std::string, math::Vec2, math::Vec2);

    // void beginWindow(std::string title, math::Vec2 pos, math::Vec2 size) {
    //     m_guiAdapter->beginWindow(std::move(title), std::move(pos), std::move(size));
    // }

    // void endWindow() {
    //     m_guiAdapter->endWindow();
    // }

private:
    std::shared_ptr<platform::gui::GUIAdapter> m_guiAdapter;
};
}