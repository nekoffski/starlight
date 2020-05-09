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

void Window::displayText(std::string text) {
    m_guiAdapter->displayText(std::move(text));
}

void Window::breakLine() {
    m_guiAdapter->breakLine();
}

void Window::sliderFloat3(std::string label, math::Vec3& v, float min, float max) {
    m_guiAdapter->sliderFloat3(std::move(label), v, min, max);
}

void Window::sliderFloat(std::string label, float& v, float min, float max) {
    m_guiAdapter->sliderFloat(std::move(label), v, min, max);
}

void Window::dragFloat3(std::string label, math::Vec3& v, float speed, float min, float max) {
    m_guiAdapter->dragFloat3(std::move(label), v, speed, min, max);
}

void Window::dragFloat(std::string label, float& v, float speed, float min, float max) {
    m_guiAdapter->dragFloat(std::move(label), v, speed, min, max);
}

bool Window::beginTreeNode(std::string label) {
    return m_guiAdapter->beginTreeNode(std::move(label));
}

void Window::popTreeNode() {
    m_guiAdapter->popTreeNode();
}

bool Window::isPreviousWidgetClicked() {
    return m_guiAdapter->isPreviousWidgetClicked();
}
}