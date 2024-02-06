#pragma once

#include <optional>

#include "starlight/core/Core.h"
#include "starlight/core/math/Glm.h"

#include "starlight/ui/UI.h"

struct UIState {
    std::optional<sl::u64> selectedEntityId;
};

static constexpr float leftComboWidthFactor = 0.15f;

static const sl::Vec3f selectedColor = { 0.1f, 0.7f, 0.1f };
static const sl::Vec3f defaultColor  = { 1.0f, 1.0f, 1.0f };
