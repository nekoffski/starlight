
#include "CollisionProcessor.h"

namespace sl::physx {

void CollisionProcessor::processCollisions(const std::vector<DynamicBody> dynamicBodies) {
    auto dynamicBodiesSize = dynamicBodies.size();

    for (int i = 0; i < dynamicBodiesSize; ++i) {
        auto& [rigidBody, transform] = dynamicBodies[i];

        auto collider = rigidBody->boundingBox->getCollider();
        collider->setModelMatrix(transform->transformation);

        for (int j = i + 1; j < dynamicBodiesSize; ++j) {
            auto& [othRigidBody, othTransform] = dynamicBodies[j];

            if (rigidBody->fixed && othRigidBody->fixed)
                continue;

            if (othRigidBody->boundingBox->collide(collider, othTransform->transformation)) {
                // if (werePreviouslyCollided(rigidBody, othRigidBody))
                // continue;

                setPreviouslyCollided(rigidBody, othRigidBody, true);

                if (rigidBody->fixed)
                    processCollisionWithFixedRigidBody(dynamicBodies[i], dynamicBodies[j]);
                else if (othRigidBody->fixed)
                    processCollisionWithFixedRigidBody(dynamicBodies[j], dynamicBodies[i]);
                else
                    processElasticCollision(dynamicBodies[i], dynamicBodies[j]);
            } else {
                setPreviouslyCollided(rigidBody, othRigidBody, false);
            }
        }
    }
}

void CollisionProcessor::processCollisionWithFixedRigidBody(const DynamicBody& fixedBody, [[maybe_unused]] const DynamicBody& dynamicBody) {
    [[maybe_unused]] auto& [rigidBody, transform] = dynamicBody;

    constexpr float decreaseFactor = 0.7f;
    rigidBody->velocity.y = -rigidBody->velocity.y * decreaseFactor;
}

void CollisionProcessor::processElasticCollision(const DynamicBody& lhs, const DynamicBody& rhs) {
    [[maybe_unused]] auto& [lhsRigidBody, lhsTransform] = lhs;
    [[maybe_unused]] auto& [rhsRigidBody, rhsTransform] = rhs;

    const auto& m1 = lhsRigidBody->mass;
    const auto& m2 = rhsRigidBody->mass;

    const auto& v1 = lhsRigidBody->velocity;
    const auto& v2 = rhsRigidBody->velocity;

    const auto massSum = m1 + m2;

    lhsRigidBody->velocity = ((m1 - m2) / massSum) * v1 + 2.0f * m2 * v2 / massSum;
    rhsRigidBody->velocity = 2.0f * m1 * v1 / massSum + ((m2 - m1) / massSum) * v2;
}

bool CollisionProcessor::werePreviouslyCollided(RigidBodyComponent* lhs, RigidBodyComponent* rhs) {
    auto& lhsId = lhs->ownerEntityId;
    auto& rhsId = rhs->ownerEntityId;

    return m_collided[lhsId].contains(rhsId) || m_collided[rhsId].contains(lhsId);
}

void CollisionProcessor::setPreviouslyCollided(RigidBodyComponent* lhs, RigidBodyComponent* rhs, bool value) {
    auto& lhsId = lhs->ownerEntityId;
    auto& rhsId = rhs->ownerEntityId;

    m_collided[lhsId][rhsId] = value;
    m_collided[rhsId][lhsId] = value;
}
}