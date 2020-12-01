#include "Panel.h"

#include "GUIProxy.h"

namespace sl::gui {
Panel::Panel(std::string title, math::Vec2 pos, math::Vec2 size,
    std::shared_ptr<platform::gui::GUIImpl> guiImpl)
    : m_title(std::move(title))
    , m_pos(std::move(pos))
    , m_size(std::move(size))
    , m_guiImpl(std::move(guiImpl)) {

    }

Panel::~Panel() {
 }

void Panel::displayText(std::string text) {
    m_guiImpl->displayText(std::move(text));
}

void Panel::breakLine() {
    m_guiImpl->breakLine();
}

bool Panel::sliderFloat3(std::string label, math::Vec3& v, float min, float max) {
    return m_guiImpl->sliderFloat3(std::move(label), v, min, max);
}

bool Panel::sliderFloat(std::string label, float& v, float min, float max) {
    return m_guiImpl->sliderFloat(std::move(label), v, min, max);
}

bool Panel::dragFloat3(std::string label, math::Vec3& v, float speed, float min, float max) {
    return m_guiImpl->dragFloat3(std::move(label), v, speed, min, max);
}

bool Panel::dragFloat(std::string label, float& v, float speed, float min, float max) {
    return m_guiImpl->dragFloat(std::move(label), v, speed, min, max);
}

bool Panel::dragInt(std::string label, int& v, float speed, int min, int max) {
    return m_guiImpl->dragInt(std::move(label), v, speed, min, max);
}

bool Panel::beginTreeNode(std::string label, PanelState state) {
    return m_guiImpl->beginTreeNode(std::move(label), state == PanelState::OPEN);
}

void Panel::popTreeNode() {
    m_guiImpl->popTreeNode();
}

bool Panel::isPreviousWidgetClicked() {
    return m_guiImpl->isPreviousWidgetClicked();
}

bool Panel::colorPicker3(std::string label, math::Vec3& color) {
    return m_guiImpl->colorPicker3(std::move(label), color);
}

bool Panel::button(std::string label, int xSize, int ySize) {
	return m_guiImpl->button(std::move(label), xSize, ySize);
}

}
