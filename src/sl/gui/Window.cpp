#include "Window.h"

#include "GUIProxy.h"

namespace sl::gui {
Window::Window(std::string title, math::Vec2 pos, math::Vec2 size,
    std::shared_ptr<platform::gui::GUIImpl> guiImpl)
    : m_title(std::move(title))
    , m_pos(std::move(pos))
    , m_size(std::move(size))
    , m_guiImpl(std::move(guiImpl)) {

    m_guiImpl->beginWindow(m_title, std::move(pos), std::move(size));
}

Window::~Window() {
    m_guiImpl->endWindow();
}

void Window::displayText(std::string text) {
    m_guiImpl->displayText(std::move(text));
}

void Window::breakLine() {
    m_guiImpl->breakLine();
}

bool Window::sliderFloat3(std::string label, math::Vec3& v, float min, float max) {
    return m_guiImpl->sliderFloat3(std::move(label), v, min, max);
}

bool Window::sliderFloat(std::string label, float& v, float min, float max) {
    return m_guiImpl->sliderFloat(std::move(label), v, min, max);
}

bool Window::dragFloat3(std::string label, math::Vec3& v, float speed, float min, float max) {
    return m_guiImpl->dragFloat3(std::move(label), v, speed, min, max);
}

bool Window::dragFloat(std::string label, float& v, float speed, float min, float max) {
    return m_guiImpl->dragFloat(std::move(label), v, speed, min, max);
}

bool Window::dragInt(std::string label, int& v, float speed, int min, int max) {
    return m_guiImpl->dragInt(std::move(label), v, speed, min, max);
}

bool Window::beginTreeNode(std::string label) {
    return m_guiImpl->beginTreeNode(std::move(label));
}

void Window::popTreeNode() {
    m_guiImpl->popTreeNode();
}

bool Window::isPreviousWidgetClicked() {
    return m_guiImpl->isPreviousWidgetClicked();
}

bool Window::colorPicker3(std::string label, math::Vec3& color) {
    return m_guiImpl->colorPicker3(std::move(label), color);
}

bool Window::button(std::string label, int xSize, int ySize) {
	return m_guiImpl->button(std::move(label), xSize, ySize);
}

}
