#include <starlight/platform/gui/impl/GUIImpl.h>

#include <starlight/core/log/Logger.h>
#include <starlight/platform/gui/impl/imgui/ImGuiImpl.h>

namespace starl::platform::gui::impl {

std::unique_ptr<GUIImpl> GUIImpl::create(void* windowHandle) {
    LOG(INFO) << "instancing imgui impl";
    return std::make_unique<imgui::ImGuiImpl>(windowHandle);
}

GUIImpl::~GUIImpl() = default;
}