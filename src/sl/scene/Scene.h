#pragma once

#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

#include "Skybox.h"
#include "sl/cam/Camera.h"
#include "sl/ecs/Registry.hpp"
#include "sl/ecs/fwd.h"
#include "sl/physx/Vector.h"

namespace sl::scene {

class Scene {
   public:
    ecs::Entity& addEntity(std::string name);

    int getEntitiesCount() const;

    void clear();

    cam::Camera* camera;
    ecs::Registry ecsRegistry;

    std::optional<Skybox> skybox;

    std::vector<physx::ColoredVector> vectors;
};
}  // namespace sl::scene
