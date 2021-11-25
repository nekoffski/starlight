#include "Utils.h"

#include <kc/core/Scope.hpp>
#include <kc/core/String.h>

namespace sl::gui {

bool textInput(const std::string& label, std::string& text) {
    static constexpr int bufferSize = 256;

    text.resize(bufferSize);
    ON_SCOPE_EXIT { kc::core::strip(text); };

    return ImGui::InputText(label.c_str(), &text[0], text.size());
}

bool labeledTextInput(const std::string& label, std::string& text, const int padding) {
    ImGui::Text("%s", label.c_str());
    ImGui::SameLine(padding);

    return textInput("##" + label, text);
}

void showImage(gfx::Texture& texture, math::Vec2 size, math::Vec2 uv0, math::Vec2 uv1) {
    ImGui::Image((void*)(intptr_t)texture.getBuffer(), ImVec2(size.x, size.y),
        ImVec2(uv0.x, uv0.y), ImVec2(uv1.x, uv1.y));
}

bool combo(const std::string& label, int* currentItem, const std::vector<std::string>& items) {
    std::vector<const char*> imguiItems;
    auto itemsSize = items.size();
    imguiItems.reserve(itemsSize);

    for (auto& item : items)
        imguiItems.push_back(item.c_str());

    return ImGui::Combo(label.c_str(), currentItem, imguiItems.data(), itemsSize);
}

void pushTextColor(const math::Vec4& color) {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(color.x, color.y, color.z, color.w));
}

void popTextColor(int count) {
    ImGui::PopStyleColor(count);
}

void beginPanel(const std::string& title, math::Vec2 position, math::Vec2 size) {
    ImGui::SetNextWindowPos(ImVec2(position.x, position.y));
    ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
    ImGui::Begin(title.c_str());
}

void beginTransparentPanel(const std::string& title, math::Vec2 position, math::Vec2 size) {
    ImGui::SetNextWindowPos(ImVec2(position.x, position.y));
    ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
    static bool opened = true;
    ImGui::Begin(title.c_str(), &opened,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
}

void endPanel() {
    ImGui::End();
}

}
