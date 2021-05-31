#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "Skybox.h"
#include "sl/ecs/Registry.hpp"
#include "sl/ecs/fwd.h"
#include "sl/gfx/camera/Camera.h"

namespace sl::scene {
struct Scene {
    static std::shared_ptr<Scene> create();

    ecs::Entity& addEntity(std::string name) {
        return ecsRegistry.createEntity(std::move(name));
    }

    int getEntitiesCount() const {
        return ecsRegistry.getEntitiesCount();
    }

    void clear() {
        skybox.reset();
        ecsRegistry.clear();
    }

    std::shared_ptr<gfx::camera::Camera> camera;
    ecs::Registry ecsRegistry;
    std::shared_ptr<Skybox> skybox;
};
}
