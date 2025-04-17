#include "RenderPassBackend.hh"

#include "starlight/core/Globals.hh"

#ifdef SL_USE_VK
#include "vulkan/VulkanDevice.hh"
#include "vulkan/VulkanRenderPassBackend.hh"
#endif

namespace sl {

kstd::UniquePtr<RenderPassBackend> RenderPassBackend::create(
  const Properties& props, bool hasPreviousPass, bool hasNextPass
) {
#ifdef SL_USE_VK
    auto& device = static_cast<vk::VulkanDevice&>(Device::get().getImpl());

    return props.type == Type::normal
             ? kstd::makeUnique<vk::VulkanRenderPassBackend>(
                 device, props, hasPreviousPass, hasNextPass
               )
             : kstd::makeUnique<vk::VulkanImguiRenderPassBackend>(
                 device, props, hasPreviousPass, hasNextPass,
                 Globals::get().getConfig().paths.fonts
               );
#else
    log::panic("GPU API vendor not specified");
#endif
}

}  // namespace sl