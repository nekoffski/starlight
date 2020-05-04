#include <starlight/gui/Window.h>

#include <starlight/gui/GUIProxy.h>

namespace starl::gui {
Window::Window(std::string title, math::Vec2 pos, math::Vec2 size,
    std::shared_ptr<platform::gui::GUIAdapter> guiAdapter)
    : m_title(std::move(title))
    , m_pos(std::move(pos))
    , m_size(std::move(size))
    , m_guiAdapter(std::move(guiAdapter)) {

    m_guiAdapter->beginWindow(m_title, std::move(pos), std::move(size));
}

Window::~Window() {
    m_guiAdapter->endWindow();
}
}