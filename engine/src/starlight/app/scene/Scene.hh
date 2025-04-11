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
    void forEach(C&& callback) {
        m_entities.forEach(std::forward<C>(callback));
    }

    template <typename ComponentType, typename C>
    requires std::derived_from<ComponentType, Component>
             && Callable<C, void, ComponentType&>
    void forEach(C&& callback) {
        m_componentManager.getContainer<ComponentType>()->forEach(
          [&]([[maybe_unused]] const auto& k, auto& v) { callback(v); }
        );
    }

    Entity& addEntity(std::optional<std::string> name = {});
    Entity* getEntity(const std::string& name);

    void setSkybox(SharedPtr<Skybox> skybox);
    void resetSkybox();
    Skybox* getSkybox();

public:
    SharedPtr<Skybox> m_skybox;
    ComponentManager m_componentManager;
    StaticVector<Entity> m_entities;
};

}  // namespace sl
