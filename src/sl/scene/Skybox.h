#pragma once

#include <memory>

#include "sl/platform/shader/Shader.h"
#include "sl/platform/texture/Cubemap.h"

namespace sl::scene {
struct Skybox {
    static std::shared_ptr<Skybox> create(std::shared_ptr<platform::shader::Shader> shader,
        std::shared_ptr<platform::texture::Cubemap> cubemap) {
        return std::make_shared<Skybox>(shader, cubemap);
    }

    std::shared_ptr<platform::shader::Shader> shader;
    std::shared_ptr<platform::texture::Cubemap> cubemap;
};
}