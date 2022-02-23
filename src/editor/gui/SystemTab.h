#pragma once

#include <memory>

#include "Widget.h"
#include "sl/core/ClockManager.h"
#include "sl/ecs/Entity.h"
#include "sl/gfx/Cubemap.h"
#include "sl/glob/Globals.h"

namespace editor::gui {

class SystemTab : public Widget {
   public:
    explicit SystemTab(std::shared_ptr<SharedState> sharedState) : Widget(sharedState) {}

    void render() override {
        using namespace sl;

        with_TabItem(ICON_FA_SITEMAP "  System") {
            with_OpenedTreeNode("Statistics") {
                auto message = fmt::format(" FPS:            {}\n Delta time: {}\n",
                                           core::ClockManager::get().getFPS(),
                                           core::ClockManager::get().getDeltaTime());
                ImGui::Text("%s", message.c_str());
            }

            ImGui::Separator();

            with_OpenedTreeNode("Renderer") {
                const auto& info = glob::Globals::get().rendererInfo;

                constexpr auto formatString =
                    "GPU Vendor:\n       {}\n\nGPU API Release:\n       {}\n\nGPU: \n       {}\n\n"
                    "Shading language version:\n       {}\n\n";

                auto message = fmt::format(formatString, info.gpuApiVendor, info.gpuApiRelease,
                                           info.rendererName, info.shadingLanguageVersion);
                ImGui::Text("%s", message.c_str());
            }
        }
    }
};
}  // namespace editor::gui
