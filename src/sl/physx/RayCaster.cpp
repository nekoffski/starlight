#include "RayCaster.h"

#include <limits>

#include "sl/rendering/utils/Misc.h"

namespace sl::physx {

std::optional<std::string> RayCaster::findIntersection(RayCaster::Args&& args) {
    std::optional<std::string> hitEntity;
    float nearestHit = std::numeric_limits<float>::max();

    for (const auto& model : args.models) {
        const auto& modelMatrix =
            sl::rendering::utils::getModelMatrix(model.ownerEntityId, args.transforms);

        if (not model.boundingBox) [[unlikely]]
            continue;

        const auto intersectRecord =
            model.boundingBox->getCollider()->intersectsWith(args.primaryRay, modelMatrix);

        if (intersectRecord.has_value() && intersectRecord->tNear < nearestHit) {
            nearestHit = intersectRecord->tNear;
            hitEntity = model.ownerEntityId;
        }
    }

    return hitEntity;
}

}  // namespace sl::physx