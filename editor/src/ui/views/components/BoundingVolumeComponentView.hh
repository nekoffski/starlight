#pragma once

#include "ComponentView.hh"

#include "starlight/app/scene/components/BoundingVolumeComponent.hh"

namespace sle {

struct BoundingVolumeComponentView : ComponentViewBase<sl::BoundingVolumeComponent> {
    using ComponentViewBase::ComponentViewBase;

    void render(sl::BoundingVolumeComponent& component) override;
};

}  // namespace sle