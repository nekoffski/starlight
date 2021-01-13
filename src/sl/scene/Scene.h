#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "Skybox.h"
#include "sl/ecs/Registry.h"
#include "sl/ecs/fwd.h"
#include "sl/graphics/camera/Camera.h"

namespace sl::scene {
struct Scene {
    static std::shared_ptr<Scene> create();

    std::shared_ptr<ecs::Entity> addEntity(std::string name) {
        return ecsRegistry.createEntity(std::move(name));
    }

    std::shared_ptr<graphics::camera::Camera> camera;
    ecs::Registry ecsRegistry;
    std::shared_ptr<Skybox> skybox;
};
}
