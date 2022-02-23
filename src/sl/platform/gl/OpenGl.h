#pragma once

#include "OpenGlCubemap.h"
#include "OpenGlElementBuffer.h"
#include "OpenGlFrameBuffer.h"
#include "OpenGlRenderApi.h"
#include "OpenGlRenderBuffer.h"
#include "OpenGlShader.h"
#include "OpenGlShaderCompiler.h"
#include "OpenGlTexture.h"
#include "OpenGlVertexArray.h"
#include "OpenGlVertexBuffer.h"
#include "sl/platform/GPU.h"

namespace sl::platform::gl {

struct OpenGl : GPU {
    explicit OpenGl()
        : GPU(std::make_unique<OpenGlVertexArrayFactory>(),
              std::make_unique<OpenGlVertexBufferFactory>(),
              std::make_unique<OpenGlElementBufferFactory>(),
              std::make_unique<OpenGlFrameBufferFactory>(),
              std::make_unique<OpenGlRenderBufferFactory>(),
              std::make_unique<OpenGlTextureFactory>(), std::make_unique<OpenGlCubemapFactory>(),
              std::make_unique<OpenGlShaderFactory>(), std::make_unique<OpenGlShaderCompiler>(),
              std::make_unique<OpenGlRenderApi>()) {}
};
}  // namespace sl::platform::gl