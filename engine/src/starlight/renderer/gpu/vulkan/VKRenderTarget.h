#pragma once

#include "starlight/renderer/gpu/RenderTarget.h"
#include "starlight/core/memory/Memory.hpp"

#include "VKDevice.h"
#include "VKContext.h"
#include "VKFramebuffer.h"

namespace sl::vk {

class VKRenderTarget : public RenderTarget {
public:
    explicit VKRenderTarget(
      u32 id, VKContext& context, VKDevice& device, VKRenderPass* renderPass,
      const Properties& props
    );

    VKFramebuffer* getFramebuffer();

    void regenerate(const Properties& properties) override;

private:
    VKContext& m_context;
    VKDevice& m_device;
    VKRenderPass* m_renderPass;

    LocalPtr<VKFramebuffer> m_framebuffer;
};

}  // namespace sl::vk
