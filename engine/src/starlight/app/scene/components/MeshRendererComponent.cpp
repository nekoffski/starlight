#include "MeshRendererComponent.hh"

#include "starlight/app/factories/MeshFactory.hh"
#include "starlight/app/factories/MaterialFactory.hh"
#include "starlight/physx/bvh/AABB.hh"
#include "BoundingVolumeComponent.hh"
#include "TransformComponent.hh"

namespace sl {

MeshRendererComponentData::MeshRendererComponentData(kstd::SharedPtr<Mesh> mesh)
    : mesh(std::move(mesh))
    , material(MaterialFactory::get().getDefault()) {}

void MeshRendererComponent::onInit() {
    auto& entity = getEntity();
    if (not entity.has<BoundingVolumeComponent>()) {
        auto aabb = std::make_unique<AABB>();
        aabb->addExtent(data().mesh->getExtent());

        entity.on<TransformComponent>([&](auto& c) {
            auto& transform = c.data();
            aabb->setTransform(transform);
            data().setTransform(transform);
        });

        entity.add<BoundingVolumeComponent>(std::move(aabb));
    }
}

void MeshRendererComponent::Parser::deserialize(
  Entity& entity, kstd::BinaryReader& br
) const {
    // const auto& meshName = json["mesh"].get<std::string>();
    // entity.add<MeshRendererComponent>(MeshFactory::get().find(meshName));
}

void MeshRendererComponent::Parser::serialize(
  const MeshRendererComponentData& data, kstd::BinaryWriter& bw
) const {
    // nlohmann::json root;
    // root["mesh"] = data.mesh->getName();
    // return root;
}

std::string MeshRendererComponent::Parser::getComponentName() const {
    return "MeshRendererComponent";
}

}  // namespace sl
