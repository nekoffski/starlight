#pragma once

#include <string_view>

namespace {
const std::string HIDDEN_LABEL_PREFIX = "##";
}

namespace starl::gui {

inline std::string createHiddenLabel(std::string label) {
    return HIDDEN_LABEL_PREFIX + std::move(label);
}
}