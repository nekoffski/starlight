#include "DebugConsoleTab.h"

#include "editor/DebugConsole.hpp"

namespace editor::gui {

void DebugConsoleTab::render() {
    if (ImGui::Button("Clear")) DebugConsole::clear();

    ImGui::Text("%s", DebugConsole::getContext().c_str());
}
}  // namespace editor::gui
