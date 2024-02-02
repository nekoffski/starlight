#include "Utils.h"

#include <imgui.h>

namespace sl::ui {

void pushFont(Font* font) { ImGui::PushFont(font->handle); }
void popFont() { ImGui::PopFont(); }

void namedScope(const std::string& name, Callback&& callback) {
    ImGui::PushID(name.c_str());
    callback();
    ImGui::PopID();
}

void separator() { ImGui::Separator(); }

float getFrameHeight() { return ImGui::GetFrameHeight(); }

}  // namespace sl::ui
