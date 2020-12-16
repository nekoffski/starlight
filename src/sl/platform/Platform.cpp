#include "Platform.h"

#include "gui/ImGuiApi.h"
#include "sl/core/Logger.h"

namespace sl::platform {

std::shared_ptr<sl::gui::GuiApi> createGuiApi(void* windowHandle) {
    SL_INFO("instancing imgui impl");
    return std::make_shared<gui::detail::ImGuiApi>(windowHandle);
}
}
