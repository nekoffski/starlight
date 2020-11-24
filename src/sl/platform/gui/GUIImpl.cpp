#include "GUIImpl.h"

#include "sl/core/log/Logger.h"
#include "sl/platform/gui/imgui/ImGuiImpl.h"

namespace sl::platform::gui {

std::unique_ptr<GUIImpl> GUIImpl::create(void* windowHandle) {
    SL_INFO("instancing imgui impl");
    return std::make_unique<imgui::ImGuiImpl>(windowHandle);
}

GUIImpl::~GUIImpl() = default;
}
