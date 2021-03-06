#pragma once

#include <memory>
#include <vector>

#include "sl/core/NotNullPtr.hpp"
#include "sl/gfx/Texture.h"
#include "sl/gfx/ViewFrustum.h"
#include "sl/math/Matrix.hpp"
#include "sl/math/Vector.hpp"

namespace sl::gui {

enum class GizmoOperation {
    translate,
    rotate,
    scale
};

enum class GizmoSystem {
    local,
    world
};

class GuiApi {
public:
    struct Factory {
        virtual std::shared_ptr<GuiApi> create(core::NotNullPtr<void> windowHandle) = 0;
        virtual ~Factory() = default;
    };

    inline static std::unique_ptr<Factory> factory = nullptr;

    bool beginTreeNodeWithCheckbox(const std::string& label, bool& checkbox) {
        bool isOpened = this->beginTreeNode(label);
        this->sameLine();
        this->checkbox("##" + label, checkbox);

        return isOpened;
    }

    virtual ~GuiApi() = default;

    virtual void pushItemWidth(float) = 0;
    virtual void popItemWidth() = 0;

    virtual float getCurrentWindowWidth() = 0;
    virtual float getCurrentWindowHeight() = 0;

    virtual void setFontScale(float scale) = 0;
    virtual void pushId(const std::string&) = 0;
    virtual void pushId(int) = 0;
    virtual void popId() = 0;

    virtual bool checkbox(const std::string&, bool&) = 0;

    virtual void showImage(gfx::Texture& texture, math::Vec2 size, math::Vec2 uv0 = { 0.0f, 0.0f }, math::Vec2 uv1 = { 1.0f, 1.0f }) = 0;

    virtual void manipulateGizmo(math::Mat4& viewMatrix, math::Mat4& projectionMatrix, math::Mat4& transformation,
        GizmoOperation op, GizmoSystem system) = 0;

    virtual void pushTextColor(const math::Vec4&) = 0;
    virtual void popColor(int count = 1) = 0;

    virtual void begin() = 0;
    virtual void end() = 0;

    virtual void separator() = 0;

    virtual bool beginMenuBar() = 0;
    virtual void endMenuBar() = 0;

    virtual bool isCapturingMouse() = 0;
    virtual bool isCapturingKeyboard() = 0;

    virtual void addFont(const std::string&, short unsigned, short unsigned) = 0;

    virtual bool beginMainMenuBar() = 0;
    virtual void endMainMenuBar() = 0;

    virtual void beginChild(const std::string&, math::Vec2 = { 0.0f, 0.0f }) = 0;
    virtual void endChild() = 0;

    virtual bool beginMenu(const std::string&) = 0;
    virtual void endMenu() = 0;

    virtual bool menuItem(const std::string&) = 0;

    virtual void setNextWindowSize(math::Vec2) = 0;
    virtual void setNextWindowPosition(math::Vec2) = 0;

    virtual bool combo(const std::string&, int&, std::vector<std::string> items) = 0;
    virtual void openPopUp(const std::string&) = 0;
    virtual void closeCurrentPopUp() = 0;
    virtual bool beginPopUp(const std::string&) = 0;
    virtual void endPopUp() = 0;
    virtual bool inputText(const std::string&, std::string&) = 0;
    virtual bool isItemHovered() = 0;
    virtual bool isMouseClicked() = 0;

    virtual void beginGroup() = 0;
    virtual void endGroup() = 0;
    virtual void sameLine(float = 0) = 0;
    virtual bool beginTabBar(const std::string&) = 0;
    virtual void endTabBar() = 0;
    virtual bool beginTabItem(const std::string&) = 0;
    virtual void endTabItem() = 0;
    virtual void beginPanel(const std::string&, math::Vec2, math::Vec2) = 0;
    virtual void beginTransparentPanel(const std::string&, math::Vec2, math::Vec2) = 0;
    virtual void endPanel() = 0;
    virtual void displayText(const std::string&) = 0;
    virtual void breakLine() = 0;
    virtual bool sliderFloat3(const std::string&, math::Vec3&, float, float) = 0;
    virtual bool sliderFloat(const std::string&, float&, float, float) = 0;
    virtual bool dragFloat3(const std::string&, math::Vec3&, float = 1.0f, float = 0.0f, float = 0.0f) = 0;
    virtual bool dragFloat(const std::string&, float&, float = 1.0f, float = 0.0f, float = 0.0f) = 0;
    virtual bool dragInt(const std::string&, int&, float = 1.0f, int = 1, int = 25) = 0;
    virtual bool beginTreeNode(const std::string&, bool = true) = 0;
    virtual void popTreeNode() = 0;
    virtual bool isPreviousWidgetClicked() = 0;
    virtual bool colorPicker3(const std::string&, math::Vec3&) = 0;
    virtual bool button(const std::string&, int xSize = 0, int ySize = 0) = 0;

    virtual void setupGizmo(const gfx::ViewFrustum::Viewport&) = 0;
    virtual bool isUsingGizmo() = 0;
};
}
