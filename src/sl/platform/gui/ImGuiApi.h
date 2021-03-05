#pragma once

#include "sl/core/types/NotNullPtr.hpp"
#include "sl/gui/GuiApi.h"

namespace sl::platform::gui::detail {

class ImGuiApi : public sl::gui::GuiApi {
public:
    explicit ImGuiApi(void*);
    ~ImGuiApi() override;

    void manipulateGizmo(math::Mat4& viewMatrix, math::Mat4& projectionMatrix, math::Mat4& transformation,
        sl::gui::GizmoOperation op, sl::gui::GizmoSystem system) override;

    bool beginMenuBar() override;
    void endMenuBar() override;

    bool beginMainMenuBar() override;
    void endMainMenuBar() override;

    bool beginMenu(const std::string&) override;
    void endMenu() override;

    bool menuItem(const std::string&) override;

    void beginChild(const std::string&) override;
    void endChild() override;
    void setNextWindowPosition(math::Vec2) override;
    void setNextWindowSize(math::Vec2) override;
    void combo(const std::string&, int&, std::vector<std::string> items) override;
    void openPopUp(const std::string&) override;
    void closeCurrentPopUp() override;
    bool beginPopUp(const std::string&) override;
    void endPopUp() override;
    void inputText(const std::string&, std::string&) override;

    void beginGroup() override;
    void endGroup() override;
    void sameLine(float) override;

    bool beginTabBar(const std::string&) override;
    void endTabBar() override;
    bool beginTabItem(const std::string&) override;
    void endTabItem() override;

    void begin() override;
    void end() override;

    bool isItemHovered() override;
    bool isMouseClicked() override;

    void beginPanel(const std::string&, math::Vec2, math::Vec2) override;
    void endPanel() override;

    void displayText(const std::string&) override;
    void breakLine() override;

    bool sliderFloat3(const std::string&, math::Vec3&, float, float) override;
    bool sliderFloat(const std::string&, float&, float, float) override;

    bool dragInt(const std::string&, int&, float, int, int) override;

    bool dragFloat3(const std::string&, math::Vec3&, float, float, float) override;
    bool dragFloat(const std::string&, float&, float, float, float) override;

    bool beginTreeNode(const std::string&, bool) override;
    void popTreeNode() override;
    bool isPreviousWidgetClicked() override;

    bool colorPicker3(const std::string&, math::Vec3&) override;

    bool button(const std::string&, int xSize, int ySize) override;

    void setupGizmo(const graphics::ViewFrustum::Viewport&) override;
    bool isUsingGizmo() override;
};

struct ImGuiApiFactory : sl::gui::GuiApi::Factory {
    std::shared_ptr<sl::gui::GuiApi> create(core::types::NotNullPtr<void> windowHandle) override {
        return std::make_shared<ImGuiApi>(windowHandle);
    }
};
}
