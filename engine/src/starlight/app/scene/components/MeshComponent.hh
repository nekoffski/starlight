#pragma once

#include "starlight/app/scene/Component.hh"
#include "starlight/app/scene/ComponentParser.hh"

#include "starlight/renderer/Mesh.hh"
#include "starlight/physx/Transform.hh"

namespace sl {

struct MeshComponentData : Transformable {
    MeshComponentData(SharedPtr<Mesh> mesh)
        : mesh(std::move(mesh)) {}

    SharedPtr<Mesh> mesh;
};

struct MeshComponent : ComponentBase<MeshComponentData> {
    using ComponentBase::ComponentBase;

    void onInit() override;

    struct Parser : ComponentParserBase<MeshComponentData> {
        void deserialize(Entity&, const nlohmann::json&) const override;
        nlohmann::json serialize(const MeshComponentData&) const override;
        std::string getComponentName() const override;
    };
};

}  // namespace sl
