#include "Utils.h"

#include <imgui.h>

namespace sl::ui {

void pushFont(Font* font) { ImGui::PushFont(font->handle); }
void popFont() { ImGui::PopFont(); }

void pushTextColor(const Vec3f& color) {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(color.x, color.y, color.z, 1.0f));
}

void popTextColor(int count) { ImGui::PopStyleColor(count); }

void namedScope(const std::string& name, Callback&& callback) {
    ImGui::PushID(name.c_str());
    callback();
    ImGui::PopID();
}

void separator() { ImGui::Separator(); }

float getFrameHeight() { return ImGui::GetFrameHeight(); }

void withColor(const Vec3f& color, Callback&& callback) {
    pushTextColor(color);
    callback();
    popTextColor();
}

}  // namespace sl::ui
