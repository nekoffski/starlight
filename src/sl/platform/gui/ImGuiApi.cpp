#include "ImGuiApi.h"

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <sys/ucontext.h>

#include <ImGuizmo.h>

#include "sl/core/Logger.h"
#include "sl/core/String.hpp"

#include "sl/math/Utils.hpp"

namespace sl::platform::gui {

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

ImGuiApi::ImGuiApi(void* windowHandle) {
    SL_INFO("creating imgui context");

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    SL_INFO("initializing glfw implementation");
    ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(windowHandle), true);

    SL_INFO("initializing opengl implementation");
    const std::string openGlVersionString = "#version 440 core";
    ImGui_ImplOpenGL3_Init(openGlVersionString.c_str());

    setStyle();
}

ImGuiApi::~ImGuiApi() {
    SL_INFO("cleaning up");
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiApi::pushItemWidth(float value) {
    ImGui::PushItemWidth(value);
}

void ImGuiApi::popItemWidth() {
    ImGui::PopItemWidth();
}

float ImGuiApi::getCurrentWindowWidth() {
    return ImGui::GetWindowWidth();
}

float ImGuiApi::getCurrentWindowHeight() {
    return ImGui::GetWindowHeight();
}

void ImGuiApi::setFontScale(float scale) {
    ImGui::SetWindowFontScale(scale);
}

void ImGuiApi::pushId(const std::string& id) {
    ImGui::PushID(id.c_str());
}

void ImGuiApi::pushId(int id) {
    ImGui::PushID(id);
}

void ImGuiApi::popId() {
    ImGui::PopID();
}

bool ImGuiApi::checkbox(const std::string& label, bool& value) {
    return ImGui::Checkbox(label.c_str(), &value);
}

void ImGuiApi::showImage(sl::gfx::Texture& texture, math::Vec2 size, math::Vec2 uv0, math::Vec2 uv1) {
    ImGui::Image((void*)(intptr_t)texture.getBuffer(), ImVec2(size.x, size.y),
        ImVec2(uv0.x, uv0.y), ImVec2(uv1.x, uv1.y));
}

void ImGuiApi::pushTextColor(const math::Vec4& color) {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(color.x, color.y, color.z, color.w));
}

void ImGuiApi::popColor(int count) {
    ImGui::PopStyleColor(count);
}

bool ImGuiApi::isCapturingMouse() {
    return ImGui::GetIO().WantCaptureMouse;
}

bool ImGuiApi::isCapturingKeyboard() {
    return ImGui::GetIO().WantCaptureKeyboard;
}

void ImGuiApi::addFont(const std::string& path, short unsigned min, short unsigned max) {
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

void ImGuiApi::manipulateGizmo(math::Mat4& viewMatrix, math::Mat4& projectionMatrix, math::Mat4& transformation,
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

void ImGuiApi::beginGroup() {
    ImGui::BeginGroup();
}
void ImGuiApi::endGroup() {
    ImGui::EndGroup();
}

bool ImGuiApi::inputText(const std::string& label, std::string& text) {
    text.resize(256);
    bool isActive = ImGui::InputText(label.c_str(), &text[0], text.size());
    core::stripString(text);

    return isActive;
}

void ImGuiApi::openPopUp(const std::string& label) {
    ImGui::OpenPopup(label.c_str());
}

bool ImGuiApi::combo(const std::string& label, int& currentItem, std::vector<std::string> items) {
    std::vector<const char*> imguiItems;
    auto itemsSize = items.size();
    imguiItems.reserve(itemsSize);

    for (auto& item : items)
        imguiItems.push_back(item.c_str());

    return ImGui::Combo(label.c_str(), &currentItem, imguiItems.data(), itemsSize);
}

void ImGuiApi::closeCurrentPopUp() {
    ImGui::CloseCurrentPopup();
}

bool ImGuiApi::beginPopUp(const std::string& label) {
    return ImGui::BeginPopupModal(label.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);
}

void ImGuiApi::endPopUp() {
    ImGui::EndPopup();
}

bool ImGuiApi::beginTabBar(const std::string& id) {
    return ImGui::BeginTabBar(id.c_str());
}

void ImGuiApi::endTabBar() {
    ImGui::EndTabBar();
}

void ImGuiApi::sameLine(float offset) {
    ImGui::SameLine(offset);
}

bool ImGuiApi::beginTabItem(const std::string& label) {
    return ImGui::BeginTabItem(label.c_str());
}

void ImGuiApi::endTabItem() {
    ImGui::EndTabItem();
}

void ImGuiApi::displayText(const std::string& text) {
    ImGui::Text("%s", text.c_str());
}

void ImGuiApi::breakLine() {
    ImGui::Separator();
}

void ImGuiApi::begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
}
void ImGuiApi::end() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiApi::setNextWindowPosition(math::Vec2 position) {
    ImGui::SetNextWindowPos(ImVec2(position.x, position.y));
}

void ImGuiApi::setNextWindowSize(math::Vec2 size) {
    ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
}

void ImGuiApi::beginChild(const std::string& id, math::Vec2 size) {
    ImGui::BeginChild(id.c_str(), ImVec2(size.x, size.y));
}

void ImGuiApi::endChild() {
    ImGui::EndChild();
}

void ImGuiApi::beginTransparentPanel(const std::string& title, math::Vec2 pos, math::Vec2 size) {
    ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y));
    ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
    static bool opened = true;
    ImGui::Begin(title.c_str(), &opened,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
}

void ImGuiApi::beginPanel(const std::string& title, math::Vec2 pos, math::Vec2 size) {
    ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y));
    ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
    ImGui::Begin(title.c_str());
}

void ImGuiApi::endPanel() {
    ImGui::End();
}

void ImGuiApi::separator() {
    ImGui::Separator();
}

bool ImGuiApi::sliderFloat3(const std::string& label, math::Vec3& v, float min, float max) {
    return ImGui::SliderFloat3(label.c_str(), &v.x, min, max);
}

bool ImGuiApi::sliderFloat(const std::string& label, float& v, float min, float max) {
    return ImGui::SliderFloat(label.c_str(), &v, min, max);
}

bool ImGuiApi::dragFloat3(const std::string& label, math::Vec3& v, float speed, float min, float max) {
    return ImGui::DragFloat3(label.c_str(), &v.x, speed, min, max);
}

bool ImGuiApi::dragFloat(const std::string& label, float& v, float speed, float min, float max) {
    return ImGui::DragFloat(label.c_str(), &v, speed, min, max);
}

bool ImGuiApi::dragInt(const std::string& label, int& v, float speed, int min, int max) {
    return ImGui::DragInt(label.c_str(), &v, speed, min, max);
}

bool ImGuiApi::beginTreeNode(const std::string& label, bool opened) {
    ImGui::SetNextTreeNodeOpen(opened, ImGuiCond_Once);
    return ImGui::TreeNode(label.c_str());
}

void ImGuiApi::popTreeNode() {
    ImGui::TreePop();
}

bool ImGuiApi::isItemHovered() {
    return ImGui::IsItemHovered();
}

bool ImGuiApi::isMouseClicked() {
    return ImGui::IsMouseClicked(0);
}

bool ImGuiApi::isPreviousWidgetClicked() {
    return ImGui::IsItemClicked();
}

bool ImGuiApi::colorPicker3(const std::string& label, math::Vec3& color) {
    return ImGui::ColorPicker3(label.c_str(), &color[0]);
}

bool ImGuiApi::button(const std::string& label, int xSize, int ySize) {
    return ImGui::Button(label.c_str(), ImVec2(xSize, ySize));
}

bool ImGuiApi::beginMenuBar() {
    return ImGui::BeginMenuBar();
}

void ImGuiApi::endMenuBar() {
    ImGui::EndMenuBar();
}

bool ImGuiApi::beginMainMenuBar() {
    return ImGui::BeginMainMenuBar();
}

void ImGuiApi::endMainMenuBar() {
    ImGui::EndMainMenuBar();
}

bool ImGuiApi::beginMenu(const std::string& label) {
    return ImGui::BeginMenu(label.c_str());
}

void ImGuiApi::endMenu() {
    ImGui::EndMenu();
}

bool ImGuiApi::menuItem(const std::string& label) {
    return ImGui::MenuItem(label.c_str());
}

void ImGuiApi::setupGizmo(const sl::gfx::ViewFrustum::Viewport& viewport) {
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetRect(viewport.beginX, 0, viewport.width, viewport.height);
}

bool ImGuiApi::isUsingGizmo() {
    return ImGuizmo::IsUsing();
}
}
