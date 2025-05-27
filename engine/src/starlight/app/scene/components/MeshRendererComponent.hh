#pragma once

#include "starlight/app/scene/Component.hh"
#include "starlight/app/scene/ComponentParser.hh"

#include "starlight/renderer/Mesh.hh"
#include "starlight/renderer/Material.hh"
#include "starlight/physx/Transform.hh"
#include "starlight/app/factories/MeshFactory.hh"

namespace sl {

struct MeshRendererComponentData : Transformable {
    MeshRendererComponentData(
      kstd::SharedPtr<Mesh> mesh = MeshFactory::get().getCube()
    );

    kstd::SharedPtr<Mesh> mesh;
    kstd::SharedPtr<Material> material;
};

struct MeshRendererComponent : ComponentBase<MeshRendererComponentData> {
    using ComponentBase::ComponentBase;

    void onInit() override;

    struct Parser : ComponentParserBase<MeshRendererComponentData> {
        void deserialize(Entity&, kstd::BinaryReader&) const override;
        void serialize(const MeshRendererComponentData&, kstd::BinaryWriter&)
          const override;
        std::string getComponentName() const override;
    };
};

}  // namespace sl
