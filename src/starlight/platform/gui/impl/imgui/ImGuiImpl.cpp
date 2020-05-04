#include <starlight/platform/gui/impl/imgui/ImGuiImpl.h>

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <starlight/core/log/Logger.h>

namespace starl::platform::gui::impl::imgui {

ImGuiImpl::ImGuiImpl(void* windowHandle) {
    LOG(INFO) << "creating imgui context";

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    LOG(INFO) << "initializing glfw implementation";
    ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(windowHandle), true);
    // TODO: this string should be defined somewhere

    LOG(INFO) << "initializing opengl implementation";
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

ImGuiImpl::~ImGuiImpl() {
    LOG(INFO) << "cleaning up";
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
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

void ImGuiImpl::beginWindow(std::string title, math::Vec2 pos, math::Vec2 size) {
    ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y));
    ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
    ImGui::Begin(title.c_str());
}

void ImGuiImpl::endWindow() {
    ImGui::End();
}
}