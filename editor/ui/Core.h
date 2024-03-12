#pragma once

#include <optional>

#include "starlight/core/Core.h"
#include "starlight/core/math/Glm.h"
#include "starlight/renderer/camera/Camera.h"
#include "starlight/renderer/Viewport.h"
#include "starlight/ui/UI.h"

enum class ResourceType { mesh = 0, texture, shader, material };

std::string resourceTypeToString(ResourceType type);

struct UIState {
    std::optional<sl::u64> selectedEntityId;
    std::optional<sl::u64> selectedResourceId;
    std::optional<ResourceType> selectedResourceType;
    std::unordered_map<sl::u64, std::unique_ptr<sl::ui::ImageHandle>> imageHandles;

    sl::Camera* camera;
    sl::Viewport* viewport;

    bool showGrid  = true;
    float gridSize = 25.0f;

    sl::ui::ImageHandle* getOrCreateImageHandle(sl::Texture* texture);
};

static constexpr float panelWidthFactor = 0.15f;

static const sl::Vec3f selectedColor = { 0.1f, 0.7f, 0.1f };
static const sl::Vec3f defaultColor  = { 1.0f, 1.0f, 1.0f };
