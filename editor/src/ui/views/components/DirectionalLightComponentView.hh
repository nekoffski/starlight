#pragma once

#include "ComponentView.hh"

#include "starlight/app/scene/Components.hh"

namespace sle {

struct DirectionalLightComponentView
    : ComponentViewBase<sl::DirectionalLightComponent> {
    using ComponentViewBase::ComponentViewBase;

    void render(sl::DirectionalLightComponent& component) override;
};

}  // namespace sle