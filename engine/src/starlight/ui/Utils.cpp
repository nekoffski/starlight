#include "Utils.hh"

#include <imgui.h>

#include <backends/imgui_impl_vulkan.h>

namespace sl {

bool checkbox(const std::string& label, bool& value) {
    return ImGui::Checkbox(label.c_str(), &value);
}

bool wasItemClicked() { return ImGui::IsItemClicked(); }

void closeCurrentPopup() { ImGui::CloseCurrentPopup(); }
void sameLine() { ImGui::SameLine(); }
void separator() { ImGui::Separator(); }
void lineBreak() { ImGui::NewLine(); }

bool button(const std::string& text, const Vec2<f32>& size) {
    return ImGui::Button(text.c_str(), { size.x, size.y });
}

float getFrameHeight() { return ImGui::GetFrameHeight(); }

}  // namespace sl
