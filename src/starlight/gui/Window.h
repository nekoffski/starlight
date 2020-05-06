#pragma once

#include <memory>
#include <starlight/math/Vector.hpp>
#include <starlight/platform/gui/GUIAdapter.hpp>

namespace starl::gui {

class GUIProxy;

class Window {
    friend GUIProxy;

public:
    ~Window();

    void displayText(std::string);
    void breakLine();
    void sliderFloat3(std::string, math::Vec3&, float, float);
    void sliderFloat(std::string, float&, float, float);
    bool beginTreeNode(std::string);
    void popTreeNode();
    bool isPreviousWidgetClicked();

private:
    Window(std::string, math::Vec2, math::Vec2, std::shared_ptr<platform::gui::GUIAdapter>);

    std::string m_title;
    math::Vec2 m_pos;
    math::Vec2 m_size;
    std::shared_ptr<platform::gui::GUIAdapter> m_guiAdapter;
};
}