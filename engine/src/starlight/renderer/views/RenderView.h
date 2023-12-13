#pragma once

#include "RendererBackend.h"
#include "RenderPass.h"
#include "Shader.h"

namespace sl {

class RenderView {
public:
    virtual void init()   = 0;
    virtual void render() = 0;

protected:
    Shader* m_shader;
};

}  // namespace sl
