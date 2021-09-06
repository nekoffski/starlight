#pragma once

#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

#include "Skybox.h"
#include "sl/ecs/Registry.hpp"
#include "sl/ecs/fwd.h"
#include "sl/gfx/camera/Camera.h"
#include "sl/physx/Vector.h"

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
        LOG_INFO("Cleaning up scene");

        skybox.reset();
        ecsRegistry.clear();
    }

    std::shared_ptr<gfx::camera::Camera> camera;
    ecs::Registry ecsRegistry;

    std::optional<Skybox> skybox;

    std::vector<physx::ColoredVector> vectors;
};
}
