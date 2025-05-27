#include "BoundingVolumeComponent.hh"

namespace sl {

void BoundingVolumeComponent::Parser::deserialize(Entity&, kstd::BinaryReader& br)
  const {}

void BoundingVolumeComponent::Parser::serialize(
  const BoundingVolumeComponent&, kstd::BinaryWriter& bw
) const {}

std::string BoundingVolumeComponent::Parser::getComponentName() const {
    return "BoundingVolumeComponent";
}

}  // namespace sl
