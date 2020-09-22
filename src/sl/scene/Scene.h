#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "Skybox.h"
#include "sl/ecs/Registry.h"
#include "sl/ecs/fwd.h"

namespace sl::scene {

class Scene {
    friend class SceneManager;

public:
    static std::shared_ptr<Scene> create();

    void setSkybox(std::shared_ptr<Skybox> skybox);
    std::shared_ptr<ecs::Entity> addEntity(std::string name);

    template <typename Component, typename Factory>
    void setComponentWrapperFactory() {
        m_ecsRegistry.setComponentWrapperFactory<Component, Factory>();
    }

private:
    std::shared_ptr<Skybox> m_skybox;
    ecs::Registry m_ecsRegistry;
};
}
