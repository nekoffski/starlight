#pragma once

#include "Categories.h"
#include "Util.h"

namespace sl::event {

DECLARE_EVENT(MouseButtonActionEvent, InputCategory) {
    int button;
    int state;
};
}  // namespace sl::event
