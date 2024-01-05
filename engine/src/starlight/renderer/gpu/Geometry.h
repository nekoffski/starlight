#pragma once

#include <string>
#include <span>

#include "starlight/core/math/Extent.h"
#include "starlight/renderer/Material.h"

namespace sl {

class Geometry {
public:
    struct Properties {
        std::string name;
        Material* material;
    };

    struct Data {
        u32 vertexSize;
        u32 vertexCount;
        void* vertexData;
        std::span<uint32_t> indices;
        Extent3 extent;
    };

    // TODO: come up with a better name for that..
    struct DataDescription {
        u32 vertexCount;
        u32 vertexElementSize;
        u64 vertexBufferOffset;
        u32 indexCount;
        u32 indexElementSize;
        u64 indexBufferOffset;

        u32 indicesTotalSize() const { return indexCount * indexElementSize; }
        u32 verticesTotalSize() const { return vertexCount * vertexElementSize; }
    };

    virtual ~Geometry() = default;

    const DataDescription& getDataDescription() const;
    const Properties& getProperties() const;
    const Extent3& getExtent() const;

    u32 getId() const;

protected:
    explicit Geometry(const Properties& props, const Data& data, u32 id);

    u32 m_id;

    Properties m_props;
    DataDescription m_dataDescription;
    Extent3 m_extent;
};

}  // namespace sl
