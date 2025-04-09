#include "Text.hh"

namespace sl {

void pushTextColor(const Vec3<f32>& color) {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(color.x, color.y, color.z, 1.0f));
}

void popTextColor(int count) { ImGui::PopStyleColor(count); }

}  // namespace sl
