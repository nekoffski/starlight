#include "PhysicsEngine.h"

#include "sl/scene/components/RigidBodyComponent.h"
#include "sl/scene/components/TransformComponent.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/core/Colors.h"

#include "sl/glob/Globals.h"

namespace sl::physx {

const math::Vec3 gravityAcceleration = {0.0f, -10.0f, 0.0f};

void PhysicsEngine::processRigidBodies(scene::Scene& scene, float deltaTime) {
    std::vector<CollisionProcessor::DynamicBody> collidableEntities;

    const auto gravity = glob::Globals::get().world.gravity;

    using namespace sl::scene::components;

    auto [models, rigidBodies, transforms] =
        scene.ecsRegistry
            .getComponentsViews<ModelComponent, RigidBodyComponent, TransformComponent>();

    for (auto& rigidBody : rigidBodies) {
        if (rigidBody.useGravity && not rigidBody.fixed) rigidBody.velocity += -gravity * deltaTime;

        auto& entityId = rigidBody.ownerEntityId;

        if (transforms.doesEntityOwnComponent(entityId)) {
            auto& transform = transforms.getByEntityId(entityId);

            transform.position += rigidBody.velocity * deltaTime;
            transform.recalculateTransformation();

            if (models.doesEntityOwnComponent(entityId) && rigidBody.enableCollisions) {
                auto boundingBox = models.getByEntityId(entityId).boundingBox.get();

                collidableEntities.emplace_back(CollisionProcessor::DynamicBody{
                    &rigidBody, &transform, boundingBox});
            }
        }
    }

    m_collisionProcessor.processCollisions(collidableEntities);
}

}  // namespace sl::physx