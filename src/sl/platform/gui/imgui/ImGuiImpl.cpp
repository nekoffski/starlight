#include "ImGuiImpl.h"

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <sys/ucontext.h>

#include "imgui/imgui.h"
#include "sl/core/log/Logger.h"

namespace sl::platform::gui::imgui {

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
    ImGui::Text(text.c_str());
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
