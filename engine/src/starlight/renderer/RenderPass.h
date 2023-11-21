#pragma once

#include <vector>

#include <starlight/core/Core.h>
#include <starlight/core/math/Glm.h>

#include "RenderTarget.h

namespace sl {

class RenderPass {
public:
private:
    u32 m_id;

    std::vector<RenderTarget*> m_renderTargets;
};

}  // namespace sl