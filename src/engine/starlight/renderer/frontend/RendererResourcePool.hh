#pragma once

#include <kcore/IdAllocator.hh>

#include "starlight/core/Config.hh"
#include "starlight/core/Core.hh"
#include "starlight/renderer/core/Primitive.hh"
#include "starlight/renderer/core/RenderResource.hh"
#include "starlight/renderer/rhi/RenderDevice.hh"

namespace sl {

struct Primitive {
    VertexLayout vertexLayout;
    DeviceBufferHandle vertexBuffer;
    DeviceBufferHandle indexBuffer;
    u32 vertexBufferOffset;
    u32 indexBufferOffset;
    u32 indexCount;
};

class RendererResourcePool : public kc::NonCopyable, public kc::NonMovable {
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
    kc::TypeIdAllocator m_idAllocator;

    std::unordered_map<PrimitiveHandle, Primitive> m_primitives;
    std::unordered_map<ShaderHandle, ShaderDescription> m_shaderDescriptions;
};

}  // namespace sl
