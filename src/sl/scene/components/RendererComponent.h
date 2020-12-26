#pragma once

#include "sl/ecs/Component.h"

#include "sl/graphics/Shader.h"

namespace sl::scene::components {

struct RendererComponent : public ecs::Component {
    explicit RendererComponent(std::shared_ptr<graphics::Shader> shader)
        : shader(shader) {
    }

    std::shared_ptr<graphics::Shader> shader;
};
}
