#pragma once

#include <string>
#include <string_view>

#include <imgui/imgui.h>

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <ImGuizmo.h>

#include "sl/core/String.hpp"
#include "sl/gfx/Texture.h"
#include "sl/math/Matrix.hpp"
#include "sl/math/Utils.hpp"
#include "sl/math/Vector.hpp"

#include "sl/gfx/ViewFrustum.h"

namespace {
const std::string HIDDEN_LABEL_PREFIX = "##";
}

namespace sl::gui {

static void setStyle() {
    ImGuiStyle& style = ImGui::GetStyle();

    // light style from Pacôme Danhiez (user itamago) https://github.com/ocornut/imgui/pull/511#issuecomment-175719267
    // style.Alpha = 0.85f;
    style.WindowRounding = 0.0f;
    style.FrameRounding = 0.0f;
    auto& colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4 { 0.1f, 0.105f, 0.11f, 1.0f };

    // Headers
    colors[ImGuiCol_Header] = ImVec4 { 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_HeaderHovered] = ImVec4 { 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_HeaderActive] = ImVec4 { 0.15f, 0.1505f, 0.151f, 1.0f };

    // Buttons
    colors[ImGuiCol_Button] = ImVec4 { 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_ButtonHovered] = ImVec4 { 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_ButtonActive] = ImVec4 { 0.15f, 0.1505f, 0.151f, 1.0f };

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4 { 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_FrameBgHovered] = ImVec4 { 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_FrameBgActive] = ImVec4 { 0.15f, 0.1505f, 0.151f, 1.0f };

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4 { 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabHovered] = ImVec4 { 0.38f, 0.3805f, 0.381f, 1.0f };
    colors[ImGuiCol_TabActive] = ImVec4 { 0.28f, 0.2805f, 0.281f, 1.0f };
    colors[ImGuiCol_TabUnfocused] = ImVec4 { 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4 { 0.2f, 0.205f, 0.21f, 1.0f };

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4 { 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgActive] = ImVec4 { 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4 { 0.15f, 0.1505f, 0.151f, 1.0f };
}

inline void setupGui(void* windowHandle) {
    LOG_INFO("creating imgui context");

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    LOG_INFO("initializing glfw implementation");
    ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(windowHandle), true);

    LOG_INFO("initializing opengl implementation");
    const std::string openGlVersionString = "#version 440 core";
    ImGui_ImplOpenGL3_Init(openGlVersionString.c_str());

    setStyle();
}

inline void shutdownGui() {
    LOG_INFO("cleaning up");
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

inline void addFont(const std::string& path, short unsigned min, short unsigned max) {
    ImGuiIO& io = ImGui::GetIO();

    constexpr int sizePixels = 15;
    io.Fonts->AddFontFromFileTTF("/home/nek0/kapik/projects/starlight/res/fonts/Roboto-Regular.ttf", sizePixels);

    // io.Fonts->AddFontDefault();

    ImFontConfig config;
    config.MergeMode = true;
    config.GlyphMinAdvanceX = 11.0f;
    config.GlyphMaxAdvanceX = 15.0f;

    // TODO: THIS CAN'T BE STATIC!!
    static const ImWchar iconRanges[] = { min, max, 0 };
    io.Fonts->AddFontFromFileTTF(path.c_str(), 15.0f, &config, iconRanges);
}

inline void begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
}
inline void end() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

enum class GizmoOperation {
    translate,
    rotate,
    scale
};

enum class GizmoSystem {
    local,
    world
};

inline void setupGizmo(const sl::gfx::ViewFrustum::Viewport& viewport) {
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetRect(viewport.beginX, 0, viewport.width, viewport.height);
}

inline bool isUsingGizmo() {
    return ImGuizmo::IsUsing();
}

inline void manipulateGizmo(math::Mat4& viewMatrix, math::Mat4& projectionMatrix, math::Mat4& transformation,
    const sl::gui::GizmoOperation op, const sl::gui::GizmoSystem system) {

    static std::unordered_map<sl::gui::GizmoOperation, ImGuizmo::OPERATION> operationLookupTable = {
        { sl::gui::GizmoOperation::translate, ImGuizmo::OPERATION::TRANSLATE },
        { sl::gui::GizmoOperation::rotate, ImGuizmo::OPERATION::ROTATE },
        { sl::gui::GizmoOperation::scale, ImGuizmo::OPERATION::SCALE }
    };

    static std::unordered_map<sl::gui::GizmoSystem, ImGuizmo::MODE> systemLookupTable = {
        { sl::gui::GizmoSystem::local, ImGuizmo::MODE::LOCAL },
        { sl::gui::GizmoSystem::world, ImGuizmo::MODE::WORLD },
    };

    ImGuizmo::Manipulate(math::valuePtr(viewMatrix), math::valuePtr(projectionMatrix), operationLookupTable[op],
        systemLookupTable[system], math::valuePtr(transformation));
}

inline std::string createHiddenLabel(std::string label) {
    return HIDDEN_LABEL_PREFIX + std::move(label);
}

inline bool inputText(const std::string& label, std::string& text) {
    text.resize(256);
    bool isActive = ImGui::InputText(label.c_str(), &text[0], text.size());
    core::stripString(text);

    return isActive;
}

inline void labeledTextInput(const std::string& label, std::string& value, const int padding) {
    ImGui::Text("%s", label.c_str());
    ImGui::SameLine(padding);
    inputText(createHiddenLabel(label), value);
}

inline void showImage(gfx::Texture& texture, math::Vec2 size, math::Vec2 uv0 = { 0.0f, 0.0f }, math::Vec2 uv1 = { 1.0f, 1.0f }) {
    ImGui::Image((void*)(intptr_t)texture.getBuffer(), ImVec2(size.x, size.y),
        ImVec2(uv0.x, uv0.y), ImVec2(uv1.x, uv1.y));
}

inline bool combo(const std::string& label, int* currentItem, std::vector<std::string> items) {
    std::vector<const char*> imguiItems;
    auto itemsSize = items.size();
    imguiItems.reserve(itemsSize);

    for (auto& item : items)
        imguiItems.push_back(item.c_str());

    return ImGui::Combo(label.c_str(), currentItem, imguiItems.data(), itemsSize);
}

inline void pushTextColor(const math::Vec4& color) {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(color.x, color.y, color.z, color.w));
}

inline void popTextColor(int count = 1) {
    ImGui::PopStyleColor(count);
}

inline void beginPanel(const std::string& title, math::Vec2 pos, math::Vec2 size) {
    ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y));
    ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
    ImGui::Begin(title.c_str());
}

inline void beginTransparentPanel(const std::string& title, math::Vec2 pos, math::Vec2 size) {
    ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y));
    ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
    static bool opened = true;
    ImGui::Begin(title.c_str(), &opened,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
}

inline void endPanel() {
    ImGui::End();
}

}
