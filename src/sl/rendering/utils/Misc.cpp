#include "Misc.h"

using namespace sl::scene::components;

namespace sl::rendering::utils {

const math::Mat4& getModelMatrix(const std::string& entityId, TransformComponent::View& transforms) {
    return transforms.doesEntityOwnComponent(entityId)
        ? transforms.getByEntityId(entityId).transformation
        : math::identityMatrix;
}

}