#include "GuiImpl.h"

#include "sl/core/Logger.h"
#include "sl/platform/gui/imgui/ImGuiImpl.h"

namespace sl::platform::gui {

std::unique_ptr<GuiImpl> GuiImpl::create(void* windowHandle) {
    SL_INFO("instancing imgui impl");
    return std::make_unique<imgui::ImGuiImpl>(windowHandle);
}

GuiImpl::~GuiImpl() = default;
}
