#pragma once

#include "sl/gui/GuiApi.h"

#include <gmock/gmock.h>

using namespace sl;
using namespace sl::gui;

struct GuiApiMock : public GuiApi {
    struct Factory : public GuiApi::Factory {
        MOCK_METHOD(std::shared_ptr<GuiApi>, create, (sl::core::NotNullPtr<void> windowHandle), (override));

        inline static Factory* instance = nullptr;
    };

    MOCK_METHOD(void, setFontScale, (float scale), (override));
    MOCK_METHOD(void, separator, (), (override));
    MOCK_METHOD(void, pushId, (const std::string&), (override));
    MOCK_METHOD(void, pushId, (int), (override));
    MOCK_METHOD(void, popId, (), (override));
    MOCK_METHOD(bool, checkbox, (const std::string&, bool&), (override));
    MOCK_METHOD(void, manipulateGizmo, (math::Mat4 & viewMatrix, math::Mat4& projectionMatrix, math::Mat4& transformation, GizmoOperation op, GizmoSystem system),
        (override));
    MOCK_METHOD(void, pushTextColor, (const math::Vec4&), (override));
    MOCK_METHOD(void, popColor, (int count), (override));
    MOCK_METHOD(void, begin, (), (override));
    MOCK_METHOD(void, end, (), (override));
    MOCK_METHOD(bool, beginMenuBar, (), (override));
    MOCK_METHOD(void, endMenuBar, (), (override));
    MOCK_METHOD(bool, isCapturingMouse, (), (override));
    MOCK_METHOD(bool, isCapturingKeyboard, (), (override));
    MOCK_METHOD(void, addFont, (const std::string&, short unsigned, short unsigned), (override));
    MOCK_METHOD(bool, beginMainMenuBar, (), (override));
    MOCK_METHOD(void, endMainMenuBar, (), (override));
    MOCK_METHOD(void, beginChild, (const std::string&), (override));
    MOCK_METHOD(void, endChild, (), (override));
    MOCK_METHOD(bool, beginMenu, (const std::string&), (override));
    MOCK_METHOD(void, endMenu, (), (override));
    MOCK_METHOD(bool, menuItem, (const std::string&), (override));
    MOCK_METHOD(void, setNextWindowSize, (math::Vec2), (override));
    MOCK_METHOD(void, setNextWindowPosition, (math::Vec2), (override));
    MOCK_METHOD(void, combo, (const std::string&, int&, std::vector<std::string> items), (override));
    MOCK_METHOD(void, openPopUp, (const std::string&), (override));
    MOCK_METHOD(void, closeCurrentPopUp, (), (override));
    MOCK_METHOD(bool, beginPopUp, (const std::string&), (override));
    MOCK_METHOD(void, endPopUp, (), (override));
    MOCK_METHOD(void, inputText, (const std::string&, std::string&), (override));
    MOCK_METHOD(bool, isItemHovered, (), (override));
    MOCK_METHOD(bool, isMouseClicked, (), (override));
    MOCK_METHOD(void, beginGroup, (), (override));
    MOCK_METHOD(void, endGroup, (), (override));
    MOCK_METHOD(void, sameLine, (float), (override));
    MOCK_METHOD(bool, beginTabBar, (const std::string&), (override));
    MOCK_METHOD(void, endTabBar, (), (override));
    MOCK_METHOD(bool, beginTabItem, (const std::string&), (override));
    MOCK_METHOD(void, endTabItem, (), (override));
    MOCK_METHOD(void, beginPanel, (const std::string&, math::Vec2, math::Vec2), (override));
    MOCK_METHOD(void, endPanel, (), (override));
    MOCK_METHOD(void, displayText, (const std::string&), (override));
    MOCK_METHOD(void, breakLine, (), (override));
    MOCK_METHOD(bool, sliderFloat3, (const std::string&, math::Vec3&, float, float), (override));
    MOCK_METHOD(bool, sliderFloat, (const std::string&, float&, float, float), (override));
    MOCK_METHOD(bool, dragFloat3, (const std::string&, math::Vec3&, float, float, float), (override));
    MOCK_METHOD(bool, dragFloat, (const std::string&, float&, float, float, float), (override));
    MOCK_METHOD(bool, dragInt, (const std::string&, int&, float, int, int), (override));
    MOCK_METHOD(bool, beginTreeNode, (const std::string&, bool), (override));
    MOCK_METHOD(void, popTreeNode, (), (override));
    MOCK_METHOD(bool, isPreviousWidgetClicked, (), (override));
    MOCK_METHOD(bool, colorPicker3, (const std::string&, math::Vec3&), (override));
    MOCK_METHOD(bool, button, (const std::string&, int xSize, int ySize), (override));
    MOCK_METHOD(void, setupGizmo, (const gfx::ViewFrustum::Viewport&), (override));
    MOCK_METHOD(bool, isUsingGizmo, (), (override));
    MOCK_METHOD(void, showImage, (gfx::Texture & texture, math::Vec2 size), (override));
};
