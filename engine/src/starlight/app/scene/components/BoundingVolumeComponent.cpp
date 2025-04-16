#include "BoundingVolumeComponent.hh"

namespace sl {

void BoundingVolumeComponent::Parser::deserialize(Entity&, const nlohmann::json&)
  const {}

nlohmann::json
  BoundingVolumeComponent::Parser::serialize(const BoundingVolumeComponent&) const {}

std::string BoundingVolumeComponent::Parser::getComponentName() const {
    return "BoundingVolumeComponent";
}

}  // namespace sl
