#include "PhysicsEngine.h"

#include "sl/glob/Globals.h"

namespace sl::physx {

const math::Vec3 gravityAcceleration = {0.0f, -10.0f, 0.0f};

// void queueVelocityVectorForBeingRendered(physx::Vector&& velocityVector, scene::Scene& scene) {
//     scene.vectors.emplace_back(physx::ColoredVector{std::move(velocityVector),
//     core::color::green});
// }

void PhysicsEngine::processRigidBodies(
    scene::components::RigidBodyComponent::View rigidBodies,
    scene::components::TransformComponent::View transforms, const BoundingBoxes& boundingBoxes,
    float deltaTime
) {
    std::vector<CollisionProcessor::DynamicBody> collidableEntities;

    const auto gravity = glob::Globals::get().world.gravity;

    for (auto& rigidBody : rigidBodies) {
        if (rigidBody.useGravity && not rigidBody.fixed) rigidBody.velocity += -gravity * deltaTime;

        // if (rigidBody.renderVelocity) {
        // }

        auto& entityId = rigidBody.ownerEntityId;

        if (transforms.doesEntityOwnComponent(entityId)) {
            auto& transform = transforms.getByEntityId(entityId);

            transform.position += rigidBody.velocity * deltaTime;
            transform.recalculateTransformation();

            if (boundingBoxes.contains(entityId) && rigidBody.enableCollisions)
                collidableEntities.emplace_back(CollisionProcessor::DynamicBody{
                    &rigidBody, &transform, boundingBoxes.at(entityId)});
        }
    }

    m_collisionProcessor.processCollisions(collidableEntities);
}

}  // namespace sl::physx