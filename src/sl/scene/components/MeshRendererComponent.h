#pragma once

#include "sl/core/Json.h"
#include "sl/ecs/Component.h"
#include "sl/ecs/Entity.h"
#include "sl/gfx/Shader.h"
#include "sl/utils/Globals.h"

namespace sl::scene::components {

struct MeshRendererComponent : ecs::Component {
    explicit MeshRendererComponent()
        : shader(GLOBALS().shaders->defaultModelShader)
        , polygonMode(STARL_FILL) {
    }

    std::shared_ptr<gfx::Shader> shader;
    unsigned int polygonMode;
};
}
