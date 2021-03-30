#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "Skybox.h"
#include "sl/ecs/Registry.h"
#include "sl/ecs/fwd.h"
#include "sl/gfx/camera/Camera.h"

namespace sl::scene {
struct Scene {
    static std::shared_ptr<Scene> create();

    std::shared_ptr<ecs::Entity> addEntity(std::string name) {
        return ecsRegistry.createEntity(std::move(name));
    }

    int getEntitiesCount() const {
        return ecsRegistry.getEntitiesCount();
    }

    std::shared_ptr<gfx::camera::Camera> camera;
    ecs::Registry ecsRegistry;
    std::shared_ptr<Skybox> skybox;
};
}
