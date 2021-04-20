#pragma once

#include "sl/core/NotNullPtr.hpp"
#include "sl/gfx/ViewFrustum.h"
#include "sl/gui/GuiApi.h"

namespace sl::platform::gui {

class ImGuiApi : public sl::gui::GuiApi {
public:
    explicit ImGuiApi(void*);
    ~ImGuiApi() override;

    void setFontScale(float scale) override;

    void pushId(const std::string&) override;
    void pushId(int) override;
    void popId() override;

    bool checkbox(const std::string&, bool&) override;

    void showImage(sl::gfx::Texture& texture, math::Vec2 size) override;

    void manipulateGizmo(math::Mat4& viewMatrix, math::Mat4& projectionMatrix, math::Mat4& transformation,
        sl::gui::GizmoOperation op, sl::gui::GizmoSystem system) override;

    void pushTextColor(const math::Vec4&) override;
    void popColor(int count = 1) override;

    bool beginMenuBar() override;
    void endMenuBar() override;

    bool beginMainMenuBar() override;
    void endMainMenuBar() override;

    bool beginMenu(const std::string&) override;
    void endMenu() override;

    void separator() override;

    bool isCapturingMouse() override;
    bool isCapturingKeyboard() override;

    void addFont(const std::string&, short unsigned, short unsigned) override;

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

    void setupGizmo(const sl::gfx::ViewFrustum::Viewport&) override;
    bool isUsingGizmo() override;
};

struct ImGuiApiFactory : sl::gui::GuiApi::Factory {
    std::shared_ptr<sl::gui::GuiApi> create(sl::core::NotNullPtr<void> windowHandle) override {
        return std::make_shared<ImGuiApi>(windowHandle);
    }
};
}
