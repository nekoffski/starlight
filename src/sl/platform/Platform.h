#pragma once

#include <memory>

#include "sl/gui/GuiApi.h"

namespace sl::platform {

std::shared_ptr<gui::GuiApi> createGuiApi(void* windowHandle);
}
