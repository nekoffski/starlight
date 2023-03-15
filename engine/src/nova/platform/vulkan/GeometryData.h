#pragma once

#include "nova/core/Id.h"

namespace nova::platform::vulkan {

static constexpr int vulkanMaxGeometryCount = 4096;

struct GeometryData {
    core::Id id;
    uint32_t generation;
    uint32_t vertexCount;
    uint32_t vertexSize;
    uint32_t vertexBufferOffset;
    uint32_t indexCount;
    uint32_t indexSize;
    uint32_t indexBufferOffset;
};

}  // namespace nova::platform::vulkan
