#pragma once

#include <map>

#include "starlight/core/Core.h"
#include "starlight/core/utils/ResourcePool.hpp"
#include "starlight/renderer/RenderPacket.h"

#include "ecs/Entity.h"
#include "ecs/ComponentContainerMap.hpp"

namespace sl {

class Scene {
public:
    static inline constexpr u32 defaultMaxEntities = 1024;

    explicit Scene(u32 maxEntities = defaultMaxEntities);

    Entity* addEntity(const std::string& name);
    Entity* addEntity();

    Entity* getEntity(const std::string& name);
    Entity* getEntity(u64 id);

    RenderPacket getRenderPacket();

    template <typename Callback>
    requires Callable<Callback, void, std::string, Entity*>
    void forEachEntity(Callback&& callback) {
        for (auto& [name, entity] : m_entitiesByName) callback(name, entity);
    }

private:
    Entity* storeEntity(Entity* entity);

    ResourcePool<Entity> m_entities;
    // TODO: implement FlatMap
    std::map<std::string, Entity*> m_entitiesByName;

    ComponentContainerMap m_componentMap;
};

}  // namespace sl
