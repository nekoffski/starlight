#pragma once

#include <memory>

#include "sl/ecs/Component.h"
#include "sl/ecs/Entity.h"

#include "sl/physx/BoundingBox.h"

namespace sl::scene::components {

class RigidBodyComponent : public ecs::Component {
    struct Memento {
        math::Vec3 velocity;
    };

public:
    explicit RigidBodyComponent()
        : useGravity(false)
        , enableCollisions(false)
        , renderBoundingBox(false)
        , fixed(false)
        , mass(1.0f)
        , velocity({ 0.0f, 0.0f, 0.0f })
        , boundingBox(nullptr)

    {
        name = "RigidBodyComponent";
    }

    void save() {
        m_memento = Memento { velocity };
    }

    void restore() {
        if (m_memento.has_value()) {
            velocity = m_memento->velocity;
            m_memento.reset();
        }
    }

    bool operator==(const RigidBodyComponent& other) {
        return ownerEntityId == other.ownerEntityId;
    }

    bool useGravity;
    bool enableCollisions;
    bool renderBoundingBox;
    bool fixed;

    float mass;

    math::Vec3 velocity;

    std::unique_ptr<physx::BoundingBox> boundingBox;

private:
    std::optional<Memento> m_memento;
};

}
