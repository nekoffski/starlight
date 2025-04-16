#pragma once

#include "starlight/app/scene/Component.hh"
#include "starlight/app/scene/ComponentParser.hh"

#include "starlight/physx/bvh/BoundingVolume.hh"

namespace sl {

struct BoundingVolumeComponentData {
    std::unique_ptr<BoundingVolume> boundingVolume;
};

struct BoundingVolumeComponent : ComponentBase<BoundingVolumeComponentData> {
    using ComponentBase::ComponentBase;

    struct Parser : ComponentParserBase<BoundingVolumeComponent> {
        void deserialize(Entity&, const nlohmann::json&) const override;
        nlohmann::json serialize(const BoundingVolumeComponent&) const override;
        std::string getComponentName() const override;
    };
};

}  // namespace sl
