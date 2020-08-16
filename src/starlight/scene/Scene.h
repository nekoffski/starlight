#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "Skybox.h"
#include "starlight/ecs/Entity.h"
#include "starlight/ecs/Registry.h"
#include "starlight/ecs/entity/Entity.h"
#include "starlight/rendering/camera/Camera.h"

namespace sl::scene {

class SceneRenderer;

class Scene {
    friend class SceneManager;

public:
    void setSkybox(std::shared_ptr<Skybox> skybox) {
        m_skybox = std::move(skybox);
    }

    std::shared_ptr<ecs::Entity> addEntity() {
        return m_ecsRegistry.createEntity();
    }

private:
    std::vector<std::shared_ptr<ecs::entity::Entity>> m_entities;
    std::shared_ptr<Skybox> m_skybox;
    ecs::Registry m_ecsRegistry;
};
}
