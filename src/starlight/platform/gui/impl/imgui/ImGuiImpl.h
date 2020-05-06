#pragma once

#include <starlight/platform/gui/impl/GUIImpl.h>

namespace starl::platform::gui::impl::imgui {
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

    void sliderFloat3(std::string, math::Vec3&, float, float) override;
    void sliderFloat(std::string, float&, float, float) override;

    bool beginTreeNode(std::string) override;
    void popTreeNode() override;
    bool isPreviousWidgetClicked() override;
};
}