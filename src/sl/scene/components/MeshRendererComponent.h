#pragma once

#include "sl/ecs/Component.h"
#include "sl/ecs/Entity.h"
#include "sl/gpu/Shader.h"
#include "sl/glob/Globals.h"

namespace sl::scene::components {

struct MeshRendererComponent : ecs::Component {
    using View = ecs::ComponentView<MeshRendererComponent>;

    explicit MeshRendererComponent()
        : shader(glob::Globals::get().shaders->defaultModelShader), polygonMode(STARL_FILL) {
        name = "MeshRendererComponent";
    }

    std::shared_ptr<gpu::Shader> shader;
    unsigned int polygonMode;

    bool isTransparentForLight = false;
};

}  // namespace sl::scene::components
