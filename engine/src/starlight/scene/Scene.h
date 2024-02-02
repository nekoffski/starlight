#pragma once

#include <map>

#include "starlight/core/Core.h"
#include "starlight/core/utils/ResourcePool.hpp"
#include "starlight/renderer/RenderPacket.h"

#include "ecs/Entity.h"

namespace sl {

class Scene {
public:
    static inline constexpr u32 defaultMaxEntities = 1024;

    explicit Scene(u32 maxEntities = defaultMaxEntities) :
        m_entities("Entities", maxEntities) {}

    Entity* addEntity(const std::string& name) {
        return storeEntity(m_entities.create(name));
    }
    Entity* addEntity() { return storeEntity(m_entities.create()); }

    Entity* getEntity(const std::string& name) { return m_entitiesByName[name]; }
    Entity* getEntity(u64 id) { return &m_entities.get(id); }

    RenderPacket getRenderPacket();

    template <typename Callback>
    requires Callable<Callback, void, std::string, Entity*>
    void forEachEntity(Callback&& callback) {
        for (auto& [name, entity] : m_entitiesByName) callback(name, entity);
    }

private:
    Entity* storeEntity(Entity* entity) {
        m_entitiesByName[entity->getName()] = entity;
        return entity;
    }

    ResourcePool<Entity> m_entities;
    // TODO: implement FlatMap
    std::map<std::string, Entity*> m_entitiesByName;
};

}  // namespace sl
