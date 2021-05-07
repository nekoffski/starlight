#pragma once

#include "sl/core/Logger.h"
#include "sl/scene/components/RigidBodyComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::physx {

const math::Vec3 gravityAcceleration = { 0.0f, -10.0f, 0.0f };

class PhysicsEngine {
public:
    void processRigidBodies(ecs::ComponentView<scene::components::RigidBodyComponent> rigidBodies,
        ecs::ComponentView<scene::components::TransformComponent> transforms, float deltaTime) {
        for (auto& rigidBody : rigidBodies) {
            if (rigidBody.useGravity)
                rigidBody.velocity += gravityAcceleration * deltaTime;

            auto& entityId = rigidBody.ownerEntityId;

            if (transforms.doesEntityOwnComponent(entityId)) {
                auto& transform = transforms.getByEntityId(entityId);

                transform.position += rigidBody.velocity * deltaTime;
                transform.recalculateTransformation();
            }
        }
    }

private:
};

}