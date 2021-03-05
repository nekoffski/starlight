#pragma once

#include <memory>
#include <vector>

#include "sl/graphics/ViewFrustum.h"
#include "sl/core/types/NotNullPtr.hpp"
#include "sl/math/Vector.hpp"

namespace sl::gui {

class GuiApi {
public:
    struct Factory {
        virtual std::shared_ptr<GuiApi> create(core::types::NotNullPtr<void> windowHandle) = 0;
    };

    inline static std::unique_ptr<Factory> factory = nullptr;

    virtual ~GuiApi() = default;

    virtual void begin() = 0;
    virtual void end() = 0;

    virtual bool beginMenuBar() = 0;
    virtual void endMenuBar() = 0;

    virtual bool beginMainMenuBar() = 0;
    virtual void endMainMenuBar() = 0;

	virtual void beginChild(const std::string&) = 0;
	virtual void endChild() = 0;

    virtual bool beginMenu(const std::string&) = 0;
    virtual void endMenu() = 0;

    virtual bool menuItem(const std::string&) = 0;

    virtual void setNextWindowSize(math::Vec2) = 0;
    virtual void setNextWindowPosition(math::Vec2) = 0;

    virtual void combo(const std::string&, int&, std::vector<std::string> items) = 0;
    virtual void openPopUp(const std::string&) = 0;
    virtual void closeCurrentPopUp() = 0;
    virtual bool beginPopUp(const std::string&) = 0;
    virtual void endPopUp() = 0;
    virtual void inputText(const std::string&, std::string&) = 0;
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

	virtual void setupGizmo(const graphics::ViewFrustum::Viewport&) = 0;
	virtual bool isUsingGizmo() = 0;

};
}
