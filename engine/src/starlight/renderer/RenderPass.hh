#pragma once

#include <vector>
#include <optional>
#include <unordered_map>

#include "starlight/core/Core.hh"
#include "starlight/core/math/Core.hh"
#include "kstd/Enum.hh"
#include <kstd/Id.hh>
#include "starlight/renderer/RenderPacket.hh"

#include "gpu/Texture.hh"
#include "gpu/CommandBuffer.hh"
#include "gpu/RenderPassBackend.hh"
#include "gpu/Shader.hh"

#include "fwd.hh"
#include "starlight/renderer/fwd.hh"
#include "starlight/renderer/Core.hh"

namespace sl {

class RenderPassBase
    : public kstd::NonMovable,
      public kstd::NamedResource<RenderPassBase, "RenderPass"> {
public:
    explicit RenderPassBase(
      Renderer& renderer, std::optional<std::string> name = {}
    );
    virtual ~RenderPassBase() = default;

    virtual void init(bool hasPreviousPass, bool hasNextPass) = 0;
    virtual void run(
      RenderPacket& packet, CommandBuffer& commandBuffer, u32 imageIndex,
      u64 frameNumber
    ) = 0;

protected:
    virtual Rect2<u32> getViewport() const;

    RenderPassBackend::Properties generateRenderPassProperties(
      Attachment attachments, ClearFlags clearFlags = ClearFlags::none,
      RenderPassBackend::Type type = RenderPassBackend::Type::normal
    );

    virtual Pipeline::Properties createPipelineProperties();

    virtual RenderPassBackend::Properties createRenderPassProperties(
      bool hasPreviousPass, bool hasNextPass
    ) = 0;

    Renderer& m_renderer;
    kstd::UniquePtr<RenderPassBackend> m_renderPassBackend;
};

class RenderPass : public RenderPassBase {
public:
    explicit RenderPass(
      Renderer& renderer, kstd::SharedPtr<Shader> shader,
      std::optional<std::string> name = {}
    );

    void init(bool hasPreviousPass, bool hasNextPass);
    void run(
      RenderPacket& packet, CommandBuffer& commandBuffer, u32 imageIndex,
      u64 frameNumber
    );

private:
    kstd::SharedPtr<Shader> m_shader;
    kstd::UniquePtr<Pipeline> m_pipeline;
    kstd::UniquePtr<ShaderDataBinder> m_shaderDataBinder;

    std::unordered_map<u32, u32> m_localDescriptorSets;

    virtual void render(
      RenderPacket& packet, CommandBuffer& commandBuffer, u32 imageIndex,
      u64 frameNumber
    ) = 0;

protected:
    u32 getLocalDescriporSetId(u32 id);

    template <typename T>
    void setPushConstant(
      CommandBuffer& commandBuffer, const std::string& name, T&& value
    ) {
        m_shaderDataBinder->setPushConstant(
          *m_pipeline, commandBuffer, name, std::forward<T>(value)
        );
    }

    void setGlobalUniforms(
      CommandBuffer& commandBuffer, u64 frameNumber, u32 imageIndex,
      ShaderDataBinder::UniformCallback&& callback
    );

    void setLocalUniforms(
      CommandBuffer& commandBuffer, u64 frameNumber, u32 id, u32 imageIndex,
      ShaderDataBinder::UniformCallback&& callback
    );

    void drawMesh(Mesh& mesh, CommandBuffer& buffer);
};

}  // namespace sl
