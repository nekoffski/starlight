#pragma once

#include <memory>

#include "impl/GUIImpl.h"

namespace sl::platform::gui {

class GUIAdapter {
public:
    GUIAdapter(misc::types::NotNullPtr<void> windowHandle)
        : m_pimpl(impl::GUIImpl::create(windowHandle)) {
    }

    void begin() {
        m_pimpl->begin();
    }

    void end() {
        m_pimpl->end();
    }

    void beginWindow(std::string title, math::Vec2 pos, math::Vec2 size) {
        m_pimpl->beginWindow(std::move(title), std::move(pos), std::move(size));
    }

    void endWindow() {
        m_pimpl->endWindow();
    }

    void displayText(std::string text) {
        m_pimpl->displayText(std::move(text));
    }

    void breakLine() {
        m_pimpl->breakLine();
    }

    void sliderFloat3(std::string label, math::Vec3& v, float min, float max) {
        m_pimpl->sliderFloat3(std::move(label), v, min, max);
    }

    void sliderFloat(std::string label, float& v, float min, float max) {
        m_pimpl->sliderFloat(std::move(label), v, min, max);
    }

    void dragFloat3(std::string label, math::Vec3& v, float speed = 1.0f, float min = 0.0f, float max = 0.0f) {
        m_pimpl->dragFloat3(std::move(label), v, speed, min, max);
    }

    void dragFloat(std::string label, float& v, float speed = 1.0f, float min = 0.0f, float max = 0.0f) {
        m_pimpl->dragFloat(std::move(label), v, speed, min, max);
    }

    void dragInt(std::string label, int& v, float speed = 1.0f, int min = 1, int max = 25) {
        m_pimpl->dragInt(std::move(label), v, speed, min, max);
    }

    bool beginTreeNode(std::string label) {
        return m_pimpl->beginTreeNode(std::move(label));
    }

    void popTreeNode() {
        m_pimpl->popTreeNode();
    }

    bool isPreviousWidgetClicked() {
        return m_pimpl->isPreviousWidgetClicked();
    }

    void colorPicker3(std::string label, math::Vec3& color) {
        m_pimpl->colorPicker3(std::move(label), color);
    }

private:
    std::unique_ptr<impl::GUIImpl> m_pimpl;
};
}