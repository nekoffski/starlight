#pragma once

#include "starlight/app/scene/Component.hh"
#include "starlight/app/scene/ComponentParser.hh"

#include "starlight/physx/Transform.hh"

namespace sl {

struct TransformComponent : ComponentBase<Transform> {
    using ComponentBase::ComponentBase;

    void onInit() override;

    struct Parser : ComponentParserBase<TransformComponent> {
        void deserialize(Entity&, kstd::BinaryReader&) const override;
        void serialize(const TransformComponent&, kstd::BinaryWriter&)
          const override;
        std::string getComponentName() const override;
    };
};

}  // namespace sl
