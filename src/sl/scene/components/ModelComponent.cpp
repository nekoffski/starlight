#include "ModelComponent.h"

#include "sl/gui/GuiStyle.h"
#include "sl/utils/Globals.h"

#include "imgui/imgui.h"

namespace sl::scene::components {

ModelComponent::ModelComponent() {
    instances.push_back(math::Vec3 { 0.0f, 0.0f, 0.0f });
}

void ModelComponent::deserialize(std::shared_ptr<ecs::Entity> entity, asset::AssetManager& assetManager, Json::Value& componentDescription) {
    auto& component = entity->addComponent<ModelComponent>();
    auto& meshes = assetManager.getMeshes();

    for (auto& meshId : componentDescription["meshes-ids"]) {
        auto id = meshId.asInt();

        auto mesh = [&]() {
            if (meshes.has(id))
                return meshes.getById(id);

            for (auto& mesh : GLOBALS().geom->meshes | std::views::values)
                if (mesh->id == id)
                    return mesh;

            // TODO: handle this case
            throw std::runtime_error { "Unhandled case" };
        }();

        component.meshes.push_back(mesh);
    }
}
}
