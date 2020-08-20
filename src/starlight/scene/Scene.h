#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "Skybox.h"
#include "starlight/ecs/Entity.h"
#include "starlight/ecs/Registry.h"
#include "starlight/rendering/camera/Camera.h"

namespace sl::scene {

class SceneRenderer;

class Scene {
    friend class SceneManager;

public:
    void setSkybox(std::shared_ptr<Skybox> skybox) {
        m_skybox = std::move(skybox);
    }

    std::shared_ptr<ecs::Entity> addEntity(std::string name) {
        return m_ecsRegistry.createEntity(std::move(name));
    }

    template <typename Component, typename Factory>
    void setComponentWrapperFactory() {
        m_ecsRegistry.setComponentWrapperFactory<Component, Factory>();
    }

private:
    std::shared_ptr<Skybox> m_skybox;
    ecs::Registry m_ecsRegistry;
};
}
