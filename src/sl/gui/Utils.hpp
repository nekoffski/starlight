#pragma once

#include <string_view>

#include "GuiApi.h"

namespace {
const std::string HIDDEN_LABEL_PREFIX = "##";
}

namespace sl::gui {

inline std::string createHiddenLabel(std::string label) {
    return HIDDEN_LABEL_PREFIX + std::move(label);
}

inline void labeledTextInput(GuiApi& gui, const std::string& label, std::string& value, const int padding) {
    gui.displayText(label);
    gui.sameLine(padding);
    gui.inputText(sl::gui::createHiddenLabel(label), value);
}
}
