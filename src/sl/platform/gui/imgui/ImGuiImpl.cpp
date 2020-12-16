#include "ImGuiImpl.h"

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <sys/ucontext.h>

#include "imgui/imgui.h"
#include "sl/core/Logger.h"

namespace sl::platform::gui::imgui {

void setStyle() {
    ImGuiStyle& style = ImGui::GetStyle();

    // light style from Pacôme Danhiez (user itamago) https://github.com/ocornut/imgui/pull/511#issuecomment-175719267
    style.Alpha = 0.85f;
    style.WindowRounding = 0.0f;
    style.FrameRounding = 0.0f;
    auto& colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

    // Headers
    colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Buttons
    colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
    colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
    colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
}

ImGuiImpl::ImGuiImpl(void* windowHandle) {
    SL_INFO("creating imgui context");

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    SL_INFO("initializing glfw implementation");
    ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(windowHandle), true);
    // TODO: this string should be defined somewhere

    SL_INFO("initializing opengl implementation");
    ImGui_ImplOpenGL3_Init("#version 330 core");

    setStyle();
}

ImGuiImpl::~ImGuiImpl() {
    SL_INFO("cleaning up");
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiImpl::beginGroup() {
    ImGui::BeginGroup();
}
void ImGuiImpl::endGroup() {
    ImGui::EndGroup();
}

void ImGuiImpl::inputText(std::string label, std::string& text) {
    text.resize(256);
    ImGui::InputText(label.c_str(), &text[0], text.size());
}

void ImGuiImpl::openPopUp(std::string label) {
    ImGui::OpenPopup(label.c_str());
}

void ImGuiImpl::combo(std::string label, int& currentItem, std::vector<std::string> items) {
    std::vector<const char*> imguiItems;
    auto itemsSize = items.size();
    imguiItems.reserve(itemsSize);

    for (auto& item : items)
        imguiItems.push_back(item.c_str());

    ImGui::Combo(label.c_str(), &currentItem, imguiItems.data(), itemsSize);
}

void ImGuiImpl::closeCurrentPopUp() {
    ImGui::CloseCurrentPopup();
}

bool ImGuiImpl::beginPopUp(std::string label) {
    return ImGui::BeginPopupModal(label.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);
}

void ImGuiImpl::endPopUp() {
    ImGui::EndPopup();
}

bool ImGuiImpl::beginTabBar(std::string id) {
    return ImGui::BeginTabBar(id.c_str());
}

void ImGuiImpl::endTabBar() {
    ImGui::EndTabBar();
}

void ImGuiImpl::sameLine(float offset) {
    ImGui::SameLine(offset);
}

bool ImGuiImpl::beginTabItem(std::string label) {
    return ImGui::BeginTabItem(label.c_str());
}

void ImGuiImpl::endTabItem() {
    ImGui::EndTabItem();
}

void ImGuiImpl::displayText(std::string text) {
    ImGui::Text("%s", text.c_str());
}

void ImGuiImpl::breakLine() {
    ImGui::Separator();
}

void ImGuiImpl::begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
void ImGuiImpl::end() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiImpl::beginPanel(std::string title, math::Vec2 pos, math::Vec2 size) {
    ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y));
    ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
    ImGui::Begin(title.c_str());
}

void ImGuiImpl::endPanel() {
    ImGui::End();
}

bool ImGuiImpl::sliderFloat3(std::string label, math::Vec3& v, float min, float max) {
    return ImGui::SliderFloat3(label.c_str(), &v.x, min, max);
}

bool ImGuiImpl::sliderFloat(std::string label, float& v, float min, float max) {
    return ImGui::SliderFloat(label.c_str(), &v, min, max);
}

bool ImGuiImpl::dragFloat3(std::string label, math::Vec3& v, float speed, float min, float max) {
    return ImGui::DragFloat3(label.c_str(), &v.x, speed, min, max);
}

bool ImGuiImpl::dragFloat(std::string label, float& v, float speed, float min, float max) {
    return ImGui::DragFloat(label.c_str(), &v, speed, min, max);
}

bool ImGuiImpl::dragInt(std::string label, int& v, float speed, int min, int max) {
    return ImGui::DragInt(label.c_str(), &v, speed, min, max);
}

bool ImGuiImpl::beginTreeNode(std::string label, bool opened) {
    ImGui::SetNextTreeNodeOpen(opened, ImGuiCond_Once);
    return ImGui::TreeNode(label.c_str());
}

void ImGuiImpl::popTreeNode() {
    ImGui::Separator();
    ImGui::TreePop();
}

bool ImGuiImpl::isItemHovered() {
    return ImGui::IsItemHovered();
}

bool ImGuiImpl::isMouseClicked() {
    return ImGui::IsMouseClicked(0);
}

bool ImGuiImpl::isPreviousWidgetClicked() {
    return ImGui::IsItemClicked();
}

bool ImGuiImpl::colorPicker3(std::string label, math::Vec3& color) {
    return ImGui::ColorPicker3(label.c_str(), &color[0]);
}

bool ImGuiImpl::button(std::string label, int xSize, int ySize) {
    return ImGui::Button(label.c_str(), ImVec2(xSize, ySize));
}
}
