#pragma once

#include <vector>

#include "starlight/core/Core.hh"
#include "starlight/window/Window.hh"
#include "starlight/core/containers/FlatMap.hh"
#include "starlight/renderer/RenderPacket.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/renderer/Skybox.hh"

#include "Entity.hh"
#include "ComponentManager.hh"

namespace sl {

class Scene : public NonCopyable {
    static constexpr u64 maxEntities = 1024;

public:
    explicit Scene();
    void clear();

    RenderPacket getRenderPacket();

    template <typename C>
    requires Callable<C, void, Entity&>
    void forEachEntity(C&& callback) {
        m_entities.forEach(std::forward<C>(callback));
    }

    Entity& addEntity(std::optional<std::string> name = {});

    void setSkybox(SharedPtr<Skybox> skybox);
    void resetSkybox();
    Skybox* getSkybox();

public:
    SharedPtr<Skybox> m_skybox;
    ComponentManager m_componentManager;
    StaticVector<Entity> m_entities;
};

}  // namespace sl
