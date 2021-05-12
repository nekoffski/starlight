#pragma once

#include "CollisionProcessor.h"
#include "sl/scene/components/RigidBodyComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::physx {

class PhysicsEngine {

public:
    void processRigidBodies(ecs::ComponentView<scene::components::RigidBodyComponent> rigidBodies,
        ecs::ComponentView<scene::components::TransformComponent> transforms, float deltaTime);

private:
    CollisionProcessor m_collisionProcessor;
};
}