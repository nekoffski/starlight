#pragma once

#include <memory>

#include "sl/math/Vector.hpp"
#include "sl/platform/gui/GUIImpl.h"

namespace sl::gui {

class GUIProxy;

class Window {
    friend GUIProxy;

public:
    ~Window();

    void displayText(std::string);
    void breakLine();
	bool sliderFloat3(std::string, math::Vec3&, float, float);
    bool sliderFloat(std::string, float&, float, float);
    bool beginTreeNode(std::string);
    void popTreeNode();
    bool isPreviousWidgetClicked();
    bool dragFloat3(std::string, math::Vec3&, float speed = 1.0f, float min = 0.0f, float max = 0.0f);
    bool dragFloat(std::string, float&, float speed = 1.0f, float min = 0.0f, float max = 0.0f);
    bool dragInt(std::string, int&, float speed = 1.0f, int min = 1, int max = 25);
    bool colorPicker3(std::string, math::Vec3&);
	bool button(std::string, int xSize=0, int ySize=0);

private:
    Window(std::string, math::Vec2, math::Vec2, std::shared_ptr<platform::gui::GUIImpl>);

    std::string m_title;
    math::Vec2 m_pos;
    math::Vec2 m_size;
    std::shared_ptr<platform::gui::GUIImpl> m_guiImpl;
};
}
