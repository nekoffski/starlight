#pragma once

#include "starlight/app/scene/Component.hh"
#include "starlight/app/scene/ComponentParser.hh"

#include "starlight/renderer/light/PointLight.hh"

namespace sl {

struct PointLightComponent : ComponentBase<PointLight> {
    using ComponentBase::ComponentBase;

    struct Parser : ComponentParserBase<PointLightComponent> {
        void deserialize(Entity&, kstd::BinaryReader&) const override;
        void serialize(const PointLightComponent&, kstd::BinaryWriter&)
          const override;
        std::string getComponentName() const override;
    };
};

}  // namespace sl
