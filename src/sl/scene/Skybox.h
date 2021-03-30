#pragma once

#include <memory>

#include "sl/gfx/Shader.h"
#include "sl/gfx/Cubemap.h"

namespace sl::scene {
struct Skybox {
    static std::shared_ptr<Skybox> create(std::shared_ptr<gfx::Shader> shader,
        std::shared_ptr<sl::gfx::Cubemap> cubemap) {
        return std::make_shared<Skybox>(shader, cubemap);
    }

    std::shared_ptr<gfx::Shader> shader;
    std::shared_ptr<sl::gfx::Cubemap> cubemap;
};
}