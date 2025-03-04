#pragma once

#include "starlight/core/Core.hh"
#include "starlight/core/Utils.hh"
#include "starlight/core/Id.hh"

#include "fwd.hh"

namespace sl::v2 {

class Entity : public NamedResource<Entity, "Entity"> {
public:
    explicit Entity(EntityManager& entityManager, OptStr name = {});

private:
    EntityManager& m_entityManager;
};

}  // namespace sl::v2
