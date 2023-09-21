#pragma once

#include "starlight/core/utils/Id.h"

namespace sl::vk {

static constexpr int vulkanMaxGeometryCount = 4096;

struct GeometryData {
    Id id;
    uint32_t generation;
    uint32_t vertexCount;
    uint32_t vertexElementSize;
    uint64_t vertexBufferOffset;
    uint32_t indexCount;
    uint32_t indexElementSize;
    uint64_t indexBufferOffset;

    uint32_t getIndicesTotalSize() const {
        return indexCount * indexElementSize;
    }
    uint32_t getVerticesTotalSize() const {
        return vertexCount * vertexElementSize;
    }
};

}  // namespace sl::vk
