#pragma once

#include <memory>

#include "sl/core/GameObject.h"
#include "sl/gpu/Cubemap.h"
#include "sl/gpu/Shader.h"

namespace sl::scene {

struct Skybox {
    std::shared_ptr<gpu::Shader> shader;
    sl::gpu::Cubemap* cubemap = nullptr;
};

}  // namespace sl::scene