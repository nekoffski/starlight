#include "ImGuiApi.h"

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <sys/ucontext.h>

#include "imgui/imgui.h"
#include "sl/core/Logger.h"

namespace sl::platform::gui::detail {

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

ImGuiApi::ImGuiApi(void* windowHandle) {
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

ImGuiApi::~ImGuiApi() {
    SL_INFO("cleaning up");
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiApi::beginGroup() {
    ImGui::BeginGroup();
}
void ImGuiApi::endGroup() {
    ImGui::EndGroup();
}

void ImGuiApi::inputText(const std::string& label, std::string& text) {
    text.resize(256);
    ImGui::InputText(label.c_str(), &text[0], text.size());
}

void ImGuiApi::openPopUp(const std::string& label) {
    ImGui::OpenPopup(label.c_str());
}

void ImGuiApi::combo(const std::string& label, int& currentItem, std::vector<std::string> items) {
    std::vector<const char*> imguiItems;
    auto itemsSize = items.size();
    imguiItems.reserve(itemsSize);

    for (auto& item : items)
        imguiItems.push_back(item.c_str());

    ImGui::Combo(label.c_str(), &currentItem, imguiItems.data(), itemsSize);
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
}
void ImGuiApi::end() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiApi::beginPanel(const std::string& title, math::Vec2 pos, math::Vec2 size) {
    ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y));
    ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
    ImGui::Begin(title.c_str());
}

void ImGuiApi::endPanel() {
    ImGui::End();
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
    ImGui::Separator();
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
}
