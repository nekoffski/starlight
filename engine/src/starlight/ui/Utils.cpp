#include "Utils.hh"

#include <imgui.h>

#include <backends/imgui_impl_vulkan.h>

namespace sl {

void verticalLine(float padding, const Vec4<f32> color, float thickness) {
    ImVec2 childMin = ImGui::GetItemRectMin();
    ImVec2 childMax = ImGui::GetItemRectMax();

    ImVec2 lineStart = ImVec2(childMax.x, childMin.y + padding);
    ImVec2 lineEnd   = ImVec2(childMax.x, childMax.y - padding);

    ImGui::GetWindowDrawList()->AddLine(
      lineStart, lineEnd, IM_COL32(color.x, color.y, color.z, color.w), thickness
    );
}

bool checkbox(const std::string& label, bool& value) {
    return ImGui::Checkbox(label.c_str(), &value);
}

void padding(const Vec2<f32>& padding) {
    ImGui::Dummy(ImVec2{ padding.x, padding.y });
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
