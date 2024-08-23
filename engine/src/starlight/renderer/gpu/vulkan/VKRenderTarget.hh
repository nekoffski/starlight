#pragma once

#include "starlight/renderer/gpu/RenderTarget.hh"
#include "starlight/core/memory/Memory.hh"

#include "VKPhysicalDevice.hh"
#include "VKContext.hh"

#include "VKContext.hh"
#include "VKFramebuffer.hh"

namespace sl::vk {

class VKRenderTarget : public RenderTarget {
public:
    explicit VKRenderTarget(
      u32 id, VKContext& context, VKLogicalDevice& device, VKRenderPass* renderPass,
      const Properties& props
    );

    VKFramebuffer* getFramebuffer();

    void regenerate(const Properties& properties) override;

private:
    VKContext& m_context;
    VKLogicalDevice& m_device;

    VKRenderPass* m_renderPass;

    LocalPtr<VKFramebuffer> m_framebuffer;
};

}  // namespace sl::vk
