#include "Utils.h"

#include <imgui.h>

namespace sl::ui {

void pushFont(Font* font) { ImGui::PushFont(font->handle); }
void popFont() { ImGui::PopFont(); }

}  // namespace sl::ui
