#pragma once

#include "sl/ecs/Component.h"

#include "sl/platform/shader/Shader.h"

namespace sl::scene::components {

struct RendererComponent : public ecs::Component {
    explicit RendererComponent(std::shared_ptr<platform::shader::Shader> shader)
        : shader(shader) {
    }

    std::shared_ptr<platform::shader::Shader> shader;
};
}
