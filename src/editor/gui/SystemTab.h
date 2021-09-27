#pragma once

#include <memory>

#include "sl/core/ClockManager.h"
#include "sl/ecs/Entity.h"
#include "sl/gfx/Cubemap.h"
#include "sl/glob/Globals.h"
#include "sl/gui/GuiApi.h"

#include "Widget.h"

namespace editor::gui {

class SystemTab : public Widget {
public:
    explicit SystemTab(std::shared_ptr<SharedState> sharedState)
        : Widget(sharedState) {
    }

    void render(sl::gui::GuiApi& gui) override {
        using namespace sl;

        if (gui.beginTabItem(ICON_FA_SITEMAP "  System")) {
            if (gui.beginTreeNode("Statistics")) {
                gui.displayText(fmt::format(" FPS:            {}\n Delta time: {}\n",
                    core::ClockManager::get()->getFPS(), core::ClockManager::get()->getDeltaTime()));

                gui.popTreeNode();
            }

            gui.breakLine();

            if (gui.beginTreeNode("Renderer")) {
                const auto& info = glob::Globals::get()->rendererInfo;

                constexpr auto formatString = "GPU Vendor:\n       {}\n\nGPU API Release:\n       {}\n\nGPU: \n       {}\n\n"
                                              "Shading language version:\n       {}\n\n";

                gui.displayText(fmt::format(formatString, info.gpuApiVendor, info.gpuApiRelease,
                    info.rendererName, info.shadingLanguageVersion));

                gui.popTreeNode();
            }

            gui.endTabItem();
        }
    }
};
}
