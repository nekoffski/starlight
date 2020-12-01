#pragma once

#include <memory>

#include "sl/math/Vector.hpp"
#include "sl/platform/gui/GUIImpl.h"

namespace sl::gui {

enum class PanelState : unsigned char {
    OPEN,
    CLOSED
};

class GUIProxy;

class Panel {
    friend GUIProxy;

public:
    ~Panel();

    void begin() {
        m_guiImpl->beginPanel(m_title, m_pos, m_size);
    }

    void end() {
        m_guiImpl->endPanel();
    }

    void displayText(std::string);
    void breakLine();
    bool sliderFloat3(std::string, math::Vec3&, float, float);
    bool sliderFloat(std::string, float&, float, float);
    bool beginTreeNode(std::string, PanelState = PanelState::OPEN);
    void popTreeNode();
    bool isPreviousWidgetClicked();
    bool dragFloat3(std::string, math::Vec3&, float speed = 1.0f, float min = 0.0f, float max = 0.0f);
    bool dragFloat(std::string, float&, float speed = 1.0f, float min = 0.0f, float max = 0.0f);
    bool dragInt(std::string, int&, float speed = 1.0f, int min = 1, int max = 25);
    bool colorPicker3(std::string, math::Vec3&);
    bool button(std::string, int xSize = 0, int ySize = 0);

private:
    Panel(std::string, math::Vec2, math::Vec2, std::shared_ptr<platform::gui::GUIImpl>);

    std::string m_title;
    math::Vec2 m_pos;
    math::Vec2 m_size;
    std::shared_ptr<platform::gui::GUIImpl> m_guiImpl;
};
}
