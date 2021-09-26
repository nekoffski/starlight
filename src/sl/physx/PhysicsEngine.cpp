#include "PhysicsEngine.h"

#include "sl/glob/Globals.h"

namespace sl::physx {

const math::Vec3 gravityAcceleration = { 0.0f, -10.0f, 0.0f };

void PhysicsEngine::processRigidBodies(ecs::ComponentView<scene::components::RigidBodyComponent> rigidBodies,
    ecs::ComponentView<scene::components::TransformComponent> transforms, float deltaTime) {

    std::vector<CollisionProcessor::DynamicBody> collidableEntities;

    for (auto& rigidBody : rigidBodies) {
        if (rigidBody.useGravity && not rigidBody.fixed)
            rigidBody.velocity += -glob::Globals::get()->world.gravity * deltaTime;

        auto& entityId = rigidBody.ownerEntityId;

        if (transforms.doesEntityOwnComponent(entityId)) {
            auto& transform = transforms.getByEntityId(entityId);

            transform.position += rigidBody.velocity * deltaTime;
            transform.recalculateTransformation();

            auto canCollide = [](auto& rigidBody) -> bool {
                return rigidBody.boundingBox != nullptr && rigidBody.enableCollisions;
            };

            if (canCollide(rigidBody))
                collidableEntities.emplace_back(
                    CollisionProcessor::DynamicBody { &rigidBody, &transform });
        }
    }

    m_collisionProcessor.processCollisions(collidableEntities);
}

}