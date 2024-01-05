#pragma once

namespace sl {

struct CommandBuffer {
    enum class State : unsigned char {
        ready,
        recording,
        inRenderPass,
        recordingEnded,
        submitted,
        notAllocated
    };

    enum class Severity : unsigned char { primary, nonPrimary };

    struct BeginFlags {
        bool isSingleUse;
        bool isRenderpassContinue;
        bool isSimultaneousUse;
    };

    virtual void begin(const BeginFlags&) = 0;
    virtual void end()                    = 0;
};

}  // namespace sl
