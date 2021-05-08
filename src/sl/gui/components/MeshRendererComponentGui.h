#pragma once

#include <unordered_map>

#include "ComponentGui.h"
#include "sl/scene/components/MeshRendererComponent.h"

namespace sl::gui::components {

class MeshRendererComponentGui : public ComponentGuiImpl<scene::components::MeshRendererComponent> {
    struct Params {
        int polygonModeItem = 0;
    };

private:
    void renderComponentGuiImpl(scene::components::MeshRendererComponent& component,
        gui::GuiApi& gui, asset::AssetManager& assetManager, ecs::Entity& entity) override;

    int getPolygonMode(int) const;

    std::vector<std::string> m_polygonModeLabels = {
        "Fill", "Lines", "Points"
    };

    std::unordered_map<std::string, Params> m_params;
};

}