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

void treeNode(const std::string& name, std::function<void(bool)>&& callback) {
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::TreeNode(name.c_str())) {
        callback(ImGui::IsItemClicked());
        ImGui::TreePop();
    }
}

bool slider(const std::string& name, Vec3f& data, const StepInterval<float>& range) {
    return ImGui::DragFloat3(
      name.c_str(), &data[0], range.step, range.min, range.max
    );
}

void closeCurrentPopup() { ImGui::CloseCurrentPopup(); }
void sameLine() { ImGui::SameLine(); }
void separator() { ImGui::Separator(); }
void lineBreak() { ImGui::NewLine(); }

bool button(const std::string& text, const Vec2f& size) {
    return ImGui::Button(text.c_str(), { size.x, size.y });
}

float getFrameHeight() { return ImGui::GetFrameHeight(); }

void withColor(const Vec3f& color, Callback&& callback) {
    pushTextColor(color);
    callback();
    popTextColor();
}

}  // namespace sl::ui
