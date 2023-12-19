#pragma once

#include "starlight/core/Core.h"
#include "starlight/renderer/fwd.h"

namespace sl {

class UIRenderer {
public:
    template <typename C, typename... Args>
    requires Callable<C, void, Args...>
    void render(CommandBuffer& commandBuffer, C&& callback, Args&&... args) {
        begin(commandBuffer);
        callback(std::forward<Args>(args)...);
        end(commandBuffer);
    }

    virtual ~UIRenderer() = default;

protected:
    virtual void begin(CommandBuffer& commandBuffer) = 0;
    virtual void end(CommandBuffer& commandBuffer)   = 0;
};

}  // namespace sl
