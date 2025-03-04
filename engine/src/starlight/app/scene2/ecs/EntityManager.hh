#pragma once

#include "starlight/core/Core.hh"
#include "starlight/core/Utils.hh"
#include "starlight/core/containers/KeyVector.hh"

#include "fwd.hh"
#include "Entity.hh"

namespace sl::v2 {

class EntityManager {
public:
    Entity& addEntity(OptStr name);
    bool hasEntity(const std::string& name);
    Entity* getEntity(const std::string& name);

private:
    KeyVector<Entity, NameGetter<Entity>> m_entities;
};

}  // namespace sl::v2
