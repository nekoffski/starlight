#pragma once

#include "sl/gui/GuiApi.h"
#include "sl/misc/types/NotNullPtr.hpp"

namespace sl::platform::gui::detail {

class ImGuiApi : public sl::gui::GuiApi {
public:
    explicit ImGuiApi(void*);
    ~ImGuiApi() override;

    void combo(std::string, int&, std::vector<std::string> items) override;
    void openPopUp(std::string) override;
    void closeCurrentPopUp() override;
    bool beginPopUp(std::string) override;
    void endPopUp() override;
    void inputText(std::string, std::string&) override;

    void beginGroup() override;
    void endGroup() override;
    void sameLine(float) override;

    bool beginTabBar(std::string) override;
    void endTabBar() override;
    bool beginTabItem(std::string) override;
    void endTabItem() override;

    void begin() override;
    void end() override;

    bool isItemHovered() override;
    bool isMouseClicked() override;

    void beginPanel(std::string, math::Vec2, math::Vec2) override;
    void endPanel() override;

    void displayText(std::string) override;
    void breakLine() override;

    bool sliderFloat3(std::string, math::Vec3&, float, float) override;
    bool sliderFloat(std::string, float&, float, float) override;

    bool dragInt(std::string, int&, float, int, int) override;

    bool dragFloat3(std::string, math::Vec3&, float, float, float) override;
    bool dragFloat(std::string, float&, float, float, float) override;

    bool beginTreeNode(std::string, bool) override;
    void popTreeNode() override;
    bool isPreviousWidgetClicked() override;

    bool colorPicker3(std::string, math::Vec3&) override;

    bool button(std::string, int xSize, int ySize) override;
};

struct ImGuiApiFactory : sl::gui::GuiApi::Factory {
    std::shared_ptr<sl::gui::GuiApi> create(misc::types::NotNullPtr<void> windowHandle) override {
        return std::make_shared<ImGuiApi>(windowHandle);
    }
};
}
