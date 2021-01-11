#include "DebugConsoleTab.h"

#include "editor/DebugConsole.hpp"

namespace editor::gui {

void DebugConsoleTab::render(sl::gui::GuiApi& gui) {
    if (gui.button("Clear"))
        DebugConsole::clear();

    gui.displayText(DebugConsole::getContext());
}
}
