#pragma once

#include "starlight/core/Config.hh"
#include "starlight/core/Core.hh"
#include "starlight/core/Id.hh"
#include "starlight/renderer/core/Primitive.hh"
#include "starlight/renderer/core/RenderResource.hh"
#include "starlight/renderer/rhi/RenderDevice.hh"

namespace sl {

struct Primitive {
    DeviceBufferHandle vertexBuffer;
    DeviceBufferHandle indexBuffer;
    u32 vertexBufferOffset;
    u32 indexBufferOffset;
    u32 indexCount;
};

class RendererResourcePool : public NonCopyable, public NonMovable {
   public:
    explicit RendererResourcePool(const Config& config, RenderDevice& device);

    Result<PrimitiveHandle> createPrimitive(
        const PrimitiveUploadData& description
    );
    void destroyPrimitive(PrimitiveHandle handle);
    Primitive* getPrimitive(PrimitiveHandle handle);

   private:
    Config m_config;
    RenderDevice& m_device;
    IdLake m_idLake;
    std::unordered_map<PrimitiveHandle, Primitive> m_primitives;
};

}  // namespace sl
