#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "starlight/ecs/entity/Entity.h"
#include "starlight/framework/graphics/camera/Camera.h"
#include "Skybox.h"

namespace sl::scene {

class SceneRenderer;

class Scene {
    friend class SceneManager;

public:
    void setSkybox(std::shared_ptr<Skybox> skybox) {
        m_skybox = std::move(skybox);
    }

    void addEntity(std::shared_ptr<ecs::entity::Entity> entity) {
        m_entities.push_back(std::move(entity));
    }

private:
    std::vector<std::shared_ptr<ecs::entity::Entity>> m_entities;
    std::shared_ptr<Skybox> m_skybox;
};
}
