#pragma once

#include "Vulkan.h"

#include "fwd.h"

namespace sl::vk {

class VKCommandBuffer {
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

    explicit VKCommandBuffer(
      const VKDevice* device, VkCommandPool commandPool,
      Severity severity = Severity::primary
    );

    ~VKCommandBuffer();

    VkCommandBufferAllocateInfo createAllocateInfo(Severity severity);

    void create(Severity severity);
    void destroy();
    void begin(const BeginFlags& args);
    void end();
    void updateSubmitted();
    void reset();

    void setState(State state);

    VkCommandBuffer getHandle() const;

    void createAndBeginSingleUse();
    void endSingleUse(VkQueue queue);

private:
    VkSubmitInfo createSubmitQueueInfo() const;
    VkCommandBufferBeginInfo createCommandBufferBeginInfo(const BeginFlags& args
    ) const;

    const VKDevice* m_device;

    VkCommandPool m_commandPool;
    VkCommandBuffer m_handle;
    State m_state;
};
}  // namespace sl::vk
