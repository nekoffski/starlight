#include "Image.h"

#include "backends/imgui_impl_vulkan.h"
#include "starlight/renderer/gpu/vulkan/VKTexture.h"

namespace sl::ui {

class VulkanImageHandle : public ImageHandle {
public:
    explicit VulkanImageHandle(Texture* texture);

    void show(const Vec2f& size, const Vec2f& minUV, const Vec2f& maxUV) override;

private:
    VkDescriptorSet createDescriptorSet(Texture* texture);
    VkDescriptorSet m_descriptorSet;
};

std::unique_ptr<ImageHandle> ImageHandle::createHandle(Texture* texture) {
    return std::make_unique<VulkanImageHandle>(texture);
}

VulkanImageHandle::VulkanImageHandle(Texture* texture) :
    m_descriptorSet(createDescriptorSet(texture)) {}

void VulkanImageHandle::show(
  const Vec2f& size, const Vec2f& minUV, const Vec2f& maxUV
) {
    ImGui::Image(
      m_descriptorSet, { size.x, size.y }, { minUV.x, minUV.y }, { maxUV.x, maxUV.y }
    );
}

VkDescriptorSet VulkanImageHandle::createDescriptorSet(Texture* texture) {
    auto vkTexture = static_cast<vk::VKTexture*>(texture);
    auto sampler   = vkTexture->getSampler();
    auto view      = vkTexture->getImage()->getView();

    return ImGui_ImplVulkan_AddTexture(
      sampler, view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    );
}

}  // namespace sl::ui
