#pragma once

#include "starlight/ecs/Component.h"

#include "starlight/platform/shader/Shader.h"

namespace sl::scene::components {

class RendererComponent : public ecs::Component {
public:
    explicit RendererComponent(std::shared_ptr<platform::shader::Shader> shader)
        : m_shader(shader) {
    }

private:
    std::shared_ptr<platform::shader::Shader> m_shader;
};
}
