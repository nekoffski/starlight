#pragma once

#include "starlight/app/scene/Component.hh"
#include "starlight/app/scene/ComponentParser.hh"

#include "starlight/renderer/Mesh.hh"
#include "starlight/renderer/Material.hh"
#include "starlight/physx/Transform.hh"

namespace sl {

struct MeshRendererComponentData : Transformable {
    MeshRendererComponentData(kstd::SharedPtr<Mesh> mesh);

    kstd::SharedPtr<Mesh> mesh;
    kstd::SharedPtr<Material> material;
};

struct MeshRendererComponent : ComponentBase<MeshRendererComponentData> {
    using ComponentBase::ComponentBase;

    void onInit() override;

    struct Parser : ComponentParserBase<MeshRendererComponentData> {
        void deserialize(Entity&, const nlohmann::json&) const override;
        nlohmann::json serialize(const MeshRendererComponentData&) const override;
        std::string getComponentName() const override;
    };
};

}  // namespace sl
