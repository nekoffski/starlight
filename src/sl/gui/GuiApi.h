#pragma once

#include <memory>
#include <vector>

#include "sl/math/Vector.hpp"
#include "sl/misc/types/NotNullPtr.hpp"

namespace sl::gui {

class GuiApi {
public:
    struct Factory {
        virtual std::shared_ptr<GuiApi> create(misc::types::NotNullPtr<void> windowHandle) = 0;
    };

    inline static std::unique_ptr<Factory> factory = nullptr;

    virtual ~GuiApi() = default;

    virtual void begin() = 0;
    virtual void end() = 0;

    virtual void combo(std::string, int&, std::vector<std::string> items) = 0;
    virtual void openPopUp(std::string) = 0;
    virtual void closeCurrentPopUp() = 0;
    virtual bool beginPopUp(std::string) = 0;
    virtual void endPopUp() = 0;
    virtual void inputText(std::string, std::string&) = 0;
    virtual bool isItemHovered() = 0;
    virtual bool isMouseClicked() = 0;

    virtual void beginGroup() = 0;
    virtual void endGroup() = 0;
    virtual void sameLine(float = 0) = 0;
    virtual bool beginTabBar(std::string) = 0;
    virtual void endTabBar() = 0;
    virtual bool beginTabItem(std::string) = 0;
    virtual void endTabItem() = 0;
    virtual void beginPanel(std::string, math::Vec2, math::Vec2) = 0;
    virtual void endPanel() = 0;
    virtual void displayText(std::string) = 0;
    virtual void breakLine() = 0;
    virtual bool sliderFloat3(std::string, math::Vec3&, float, float) = 0;
    virtual bool sliderFloat(std::string, float&, float, float) = 0;
    virtual bool dragFloat3(std::string, math::Vec3&, float = 1.0f, float = 0.0f, float = 0.0f) = 0;
    virtual bool dragFloat(std::string, float&, float = 1.0f, float = 0.0f, float = 0.0f) = 0;
    virtual bool dragInt(std::string, int&, float = 1.0f, int = 1, int = 25) = 0;
    virtual bool beginTreeNode(std::string, bool = true) = 0;
    virtual void popTreeNode() = 0;
    virtual bool isPreviousWidgetClicked() = 0;
    virtual bool colorPicker3(std::string, math::Vec3&) = 0;
    virtual bool button(std::string, int xSize = 0, int ySize = 0) = 0;
};
}
