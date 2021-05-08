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
    void save() {
        m_memento = Memento { velocity };
    }

    void restore() {
        if (m_memento.has_value()) {
            velocity = m_memento->velocity;
            m_memento.reset();
        }
    }

    bool useGravity = false;
    bool enableCollisions = false;
    bool renderBoundingBox = false;

    float mass = { 0.0f };

    math::Vec3 velocity = { 0.0f, 0.0f, 0.0f };

    std::unique_ptr<physx::BoundingBox> boundingBox = nullptr;

private:
    std::optional<Memento> m_memento;
};

}
