#pragma once

#include "sl/platform/gui/impl/GUIImpl.h"

namespace sl::platform::gui::impl::imgui {
class ImGuiImpl : public GUIImpl {
public:
    explicit ImGuiImpl(void*);
    virtual ~ImGuiImpl() override;

    void begin() override;
    void end() override;

    void beginWindow(std::string, math::Vec2, math::Vec2) override;
    void endWindow() override;

    void displayText(std::string) override;
    void breakLine() override;

    bool sliderFloat3(std::string, math::Vec3&, float, float) override;
    bool sliderFloat(std::string, float&, float, float) override;

    bool dragInt(std::string, int&, float, int, int) override;

    bool dragFloat3(std::string, math::Vec3&, float, float, float) override;
    bool dragFloat(std::string, float&, float, float, float) override;

    bool beginTreeNode(std::string) override;
    void popTreeNode() override;
    bool isPreviousWidgetClicked() override;

    bool colorPicker3(std::string, math::Vec3&) override;
};
}
