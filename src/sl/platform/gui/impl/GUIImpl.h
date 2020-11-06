#pragma once

#include <memory>

#include "sl/math/Vector.hpp"
#include "sl/misc/types/NotNullPtr.hpp"

namespace sl::platform::gui::impl {

class GUIImpl {
public:
    static std::unique_ptr<GUIImpl> create(void*);
    virtual ~GUIImpl();

    virtual void begin() = 0;
    virtual void end() = 0;

    virtual void beginWindow(std::string, math::Vec2, math::Vec2) = 0;
    virtual void endWindow() = 0;
    virtual void displayText(std::string) = 0;
    virtual void breakLine() = 0;
    virtual bool sliderFloat3(std::string, math::Vec3&, float, float) = 0;
    virtual bool sliderFloat(std::string, float&, float, float) = 0;
    virtual bool dragFloat3(std::string, math::Vec3&, float, float, float) = 0;
    virtual bool dragFloat(std::string, float&, float, float, float) = 0;
    virtual bool dragInt(std::string, int&, float, int, int) = 0;
    virtual bool beginTreeNode(std::string) = 0;
    virtual void popTreeNode() = 0;
    virtual bool isPreviousWidgetClicked() = 0;
    virtual bool colorPicker3(std::string, math::Vec3&) = 0;
};
}
