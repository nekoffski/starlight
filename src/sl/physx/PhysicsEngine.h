#pragma once

#include <ranges>

#include "sl/core/Logger.h"
#include "sl/scene/components/RigidBodyComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::physx {

const math::Vec3 gravityAcceleration = { 0.0f, -10.0f, 0.0f };

class PhysicsEngine {
public:
    void processRigidBodies(ecs::ComponentView<scene::components::RigidBodyComponent> rigidBodies,
        ecs::ComponentView<scene::components::TransformComponent> transforms, float deltaTime) {

        using scene::components::TransformComponent;

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

        auto areCollisionsEnabled = [](auto& rigidBody) -> bool { return rigidBody.boundingBox != nullptr && rigidBody.enableCollisions; };
        auto rigidBodiesWithEnabledCollisions = rigidBodies | std::views::filter(areCollisionsEnabled);

        for (auto& rigidBody : rigidBodiesWithEnabledCollisions) {
            auto& entityId = rigidBody.ownerEntityId;

            auto collider = rigidBody.boundingBox->getCollider();

            if (transforms.doesEntityOwnComponent(entityId))
                collider->setModelMatrix(transforms.getByEntityId(entityId).transformation);

            for (auto& rigidBodyToCollide : rigidBodiesWithEnabledCollisions) {
                if (rigidBody == rigidBodyToCollide)
                    continue;

                auto& entityId = rigidBodyToCollide.ownerEntityId;
                auto transform =
                    transforms.doesEntityOwnComponent(entityId) ? transforms.getByEntityId(entityId).transformation : sl::math::identityMatrix;

                if (rigidBodyToCollide.boundingBox->collide(collider, transform))
                    rigidBody.velocity.y = -rigidBody.velocity.y / 2.0f;
            }
        }
    }
};

}