#pragma once

#include <memory>

#include <starlight/platform/shader/Shader.h>
#include <starlight/platform/texture/Cubemap.h>

namespace starl::scene {
struct Skybox {
    std::shared_ptr<platform::shader::Shader> shader;
    std::shared_ptr<platform::texture::Cubemap> cubemap;
};
}