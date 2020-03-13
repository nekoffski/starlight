#pragma once

#include <memory>

namespace starl::platform::gpu {

class RenderAPI {
public:
    static std::unique_ptr<RenderAPI> create();

    virtual void drawArrays(unsigned, unsigned, unsigned) = 0;
    virtual void clearColor(float, float, float, float) = 0;
};
}