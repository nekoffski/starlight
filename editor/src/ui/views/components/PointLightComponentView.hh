#pragma once

#include "ComponentView.hh"

#include "starlight/app/scene/Components.hh"

namespace sle {

struct PointLightComponentView : ComponentViewBase<sl::PointLightComponent> {
    using ComponentViewBase::ComponentViewBase;

    void render(sl::PointLightComponent& component) override;
};

}  // namespace sle