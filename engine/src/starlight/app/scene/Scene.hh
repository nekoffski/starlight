#pragma once

#include <vector>

#include <kstd/containers/SlotBuffer.hh>

#include "starlight/core/Core.hh"
#include "starlight/window/Window.hh"
#include "starlight/renderer/RenderPacket.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/renderer/Skybox.hh"

#include "Entity.hh"
#include "ComponentManager.hh"

namespace sl {

class Scene : public kstd::NonCopyable {
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
          [&]([[maybe_unused]] const auto& k, ComponentType& v) { callback(v); }
        );
    }

    Entity& addEntity(std::optional<std::string> name = {});
    Entity* getEntity(const std::string& name);

    void setSkybox(kstd::SharedPtr<Skybox> skybox);
    void resetSkybox();
    Skybox* getSkybox();

public:
    kstd::SharedPtr<Skybox> m_skybox;
    ComponentManager m_componentManager;
    kstd::StackSlotBuffer<Entity, maxEntities> m_entities;
};

}  // namespace sl
