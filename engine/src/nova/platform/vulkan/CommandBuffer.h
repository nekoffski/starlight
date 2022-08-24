#pragma once

#include "Vulkan.h"

#include "fwd.h"

namespace nova::platform::vulkan {

class CommandBuffer {
   public:
    enum class State : unsigned char {
        ready,
        recording,
        inRenderPass,
        recordingEnded,
        submitted,
        notAllocated
    };

    struct BeginFlags {
        bool isSingleUse;
        bool isRenderpassContinue;
        bool isSimultaneousUse;
    };

    enum class Severity : unsigned char { primary, nonPrimary };

    explicit CommandBuffer(const Device* device, VkCommandPool commandPool, Severity severity);
    ~CommandBuffer();

    VkCommandBufferAllocateInfo createAllocateInfo(Severity severity);

    void create(Severity severity);
    void destroy();
    void begin(const BeginFlags& args);
    void end();
    void updateSubmitted();
    void reset();

    void setState(State state);

    VkCommandBuffer getHandle() const;

    void createAndBeginSingleUse(const Device& device, const VkCommandPool& pool);
    void endSingleUse(const Device& device, const VkCommandPool& pool, VkQueue queue);

   private:
    VkSubmitInfo createSubmitQueueInfo() const;
    VkCommandBufferBeginInfo createCommandBufferBeginInfo(const BeginFlags& args) const;

    const Device* m_device;

    VkCommandPool m_commandPool;
    VkCommandBuffer m_handle;
    State m_state;
};
}  // namespace nova::platform::vulkan
