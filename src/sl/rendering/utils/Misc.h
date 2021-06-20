#pragma once

#include <string>

#include "sl/ecs/ComponentView.hpp"
#include "sl/scene/components/TransformComponent.h"

namespace sl::rendering::utils {

const math::Mat4& getModelMatrix(const std::string& entityId, scene::components::TransformComponent::View& transforms);

}