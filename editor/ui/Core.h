#pragma once

#include <optional>

#include "starlight/core/Core.h"
#include "starlight/core/math/Glm.h"

#include "starlight/ui/UI.h"

enum class ResourceType { mesh = 0, texture, shader };

std::string resouceTypeToString(ResourceType type);

struct UIState {
    std::optional<sl::u64> selectedEntityId;
    std::optional<sl::u64> selectedResourceId;
    std::optional<ResourceType> selectedResourceType;
};

static constexpr float leftComboWidthFactor = 0.15f;

static const sl::Vec3f selectedColor = { 0.1f, 0.7f, 0.1f };
static const sl::Vec3f defaultColor  = { 1.0f, 1.0f, 1.0f };
