#pragma once

#include <string>
#include <string_view>

#include <imgui/imgui.h>

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <ImGuizmo.h>

#include "sl/gfx/Texture.h"
#include "sl/math/Matrix.hpp"
#include "sl/math/Utils.hpp"
#include "sl/math/Vector.hpp"

#include "sl/gfx/ViewFrustum.h"

#define with_OpenedTreeNode(...)                      \
    ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once); \
    with_TreeNode(__VA_ARGS__)

namespace sl::gui {

bool textInput(const std::string& label, std::string& text);
bool labeledTextInput(const std::string& label, std::string& value, const int padding);

void showImage(gfx::Texture& texture, math::Vec2 size, math::Vec2 uv0 = { 0.0f, 0.0f }, math::Vec2 uv1 = { 1.0f, 1.0f });

bool combo(const std::string& label, int* currentItem, const std::vector<std::string>& items);

void pushTextColor(const math::Vec4& color);
void popTextColor(int count = 1);

void beginPanel(const std::string& title, math::Vec2 pos, math::Vec2 size);
void beginTransparentPanel(const std::string& title, math::Vec2 pos, math::Vec2 size);
void endPanel();

}
