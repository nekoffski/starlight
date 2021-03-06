#pragma once

#include "sl/core/Json.h"
#include "sl/ecs/Component.h"
#include "sl/ecs/Entity.h"
#include "sl/gfx/Shader.h"
#include "sl/utils/Globals.h"

namespace sl::scene::components {

struct MeshRendererComponent : ecs::Component {
    using View = ecs::ComponentView<MeshRendererComponent>;

    explicit MeshRendererComponent()
        : shader(GLOBALS().shaders->defaultModelShader)
        , polygonMode(STARL_FILL) {

        name = "MeshRendererComponent";
    }

    std::shared_ptr<gfx::Shader> shader;
    unsigned int polygonMode;

    bool isTransparentForLight = false;
};

}
