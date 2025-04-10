#pragma once

#include "ComponentView.hh"

#include "starlight/app/scene/Components.hh"

namespace sle {

struct TransformComponentView : ComponentViewBase<sl::TransformComponent> {
    using ComponentViewBase::ComponentViewBase;

    void render(sl::TransformComponent& component) override;
};

}  // namespace sle