#pragma once

#include <memory>

#include "sl/core/GameObject.h"
#include "sl/gfx/Cubemap.h"
#include "sl/gfx/Shader.h"

namespace sl::scene {
struct Skybox {
    std::shared_ptr<gfx::Shader> shader;
    core::GameObjectPtr<sl::gfx::Cubemap> cubemap;
};
}