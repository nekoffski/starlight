#pragma once

#include <memory>

#include "sl/graphics/Shader.h"
#include "sl/graphics/Cubemap.h"

namespace sl::scene {
struct Skybox {
    static std::shared_ptr<Skybox> create(std::shared_ptr<graphics::Shader> shader,
        std::shared_ptr<sl::graphics::Cubemap> cubemap) {
        return std::make_shared<Skybox>(shader, cubemap);
    }

    std::shared_ptr<graphics::Shader> shader;
    std::shared_ptr<sl::graphics::Cubemap> cubemap;
};
}