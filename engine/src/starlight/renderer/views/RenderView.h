#pragma once

#include "RendererBackend.h"

namespace sl {

class RenderView {
public:
    virtual void init()   = 0;
    virtual void render() = 0;

private:
};

}  // namespace sl
