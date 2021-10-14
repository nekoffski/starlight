#pragma once

#include <memory>
#include <unordered_map>

#include "ComponentGui.h"
#include "sl/scene/components/ModelComponent.h"

namespace sl::gui::components {

class ModelComponentGui : public ComponentGuiImpl<scene::components::ModelComponent> {
    struct Params {
        std::string selectedMeshName = "";
        std::weak_ptr<geom::Mesh> selectedMesh;
    };

    using MeshesMap = std::unordered_map<std::string, std::shared_ptr<geom::Mesh>>;

private:
    void renderComponentGuiImpl(scene::components::ModelComponent& component,
        asset::AssetManager& assetManager, ecs::Entity& entity) override;

    void showMeshesSection(const std::string& header, Params& params, MeshesMap&);

    std::unordered_map<std::string, Params> m_params;
};

}