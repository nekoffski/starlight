#include "GUIImpl.h"

#include "starlight/core/log/Logger.h"
#include "starlight/platform/gui/impl/imgui/ImGuiImpl.h"

namespace sl::platform::gui::impl {

std::unique_ptr<GUIImpl> GUIImpl::create(void* windowHandle) {
    SL_INFO("instancing imgui impl");
    return std::make_unique<imgui::ImGuiImpl>(windowHandle);
}

GUIImpl::~GUIImpl() = default;
}