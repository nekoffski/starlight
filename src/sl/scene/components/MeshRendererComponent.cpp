#include "MeshRendererComponent.h"

#include "sl/utils/Globals.h"

namespace sl::scene::components {

MeshRendererComponent::MeshRendererComponent()
    : shader(GLOBALS().shaders->defaultModelShader)
    , polygonMode(STARL_FILL) {
}

void MeshRendererComponent::deserialize(std::shared_ptr<ecs::Entity> entity, asset::AssetManager& assetManager, Json::Value& componentDescription) {
    auto& component = entity->addComponent<MeshRendererComponent>();

    auto& defaultShader = GLOBALS().shaders->defaultModelShader;

    if (defaultShader->id == componentDescription["shader-id"].asInt())
        component.shader = defaultShader;
}
}
