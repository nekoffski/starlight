#pragma once

#include "sl/scene/components/RigidBodyComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::physx {

using namespace scene::components;

class CollisionProcessor {
public:
    struct DynamicBody {
        RigidBodyComponent* rigidBody;
        TransformComponent* transform;
    };

    void processCollisions(const std::vector<DynamicBody> dynamicBodies);

private:
    bool werePreviouslyCollided(RigidBodyComponent* lhs, RigidBodyComponent* rhs);
    void setPreviouslyCollided(RigidBodyComponent* lhs, RigidBodyComponent* rhs, bool value);

    std::unordered_map<std::string, std::unordered_map<std::string, bool>> m_collided;

    void processCollisionWithFixedRigidBody(const DynamicBody& fixedBody, [[maybe_unused]] const DynamicBody& dynamicBody);
    void processElasticCollision(const DynamicBody& lhs, const DynamicBody& rhs);
};

}