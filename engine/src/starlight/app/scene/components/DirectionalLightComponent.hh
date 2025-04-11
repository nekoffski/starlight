#pragma once

#include "starlight/app/scene/Component.hh"
#include "starlight/app/scene/ComponentParser.hh"

#include "starlight/renderer/light/DirectionalLight.hh"

namespace sl {

struct DirectionalLightComponent : ComponentBase<DirectionalLight> {
    using ComponentBase::ComponentBase;

    struct Parser : ComponentParserBase<DirectionalLightComponent> {
        void deserialize(Entity&, const nlohmann::json&) const override;
        nlohmann::json serialize(const DirectionalLightComponent&) const override;
        std::string getComponentName() const override;
    };
};

}  // namespace sl
