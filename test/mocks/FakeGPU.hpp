#pragma once

#include "CubemapMock.hpp"
#include "ElementBufferMock.hpp"
#include "FrameBufferMock.hpp"
#include "RenderApiMock.hpp"
#include "RenderBufferMock.hpp"
#include "ShaderCompilerMock.hpp"
#include "ShaderMock.hpp"
#include "TextureMock.hpp"
#include "VertexArrayMock.hpp"
#include "VertexBufferMock.hpp"
#include "sl/platform/GPU.h"

using namespace sl;

struct FakeGPU : platform::GPU {
    FakeGPU()
        : GPU(std::make_unique<VertexArrayMock::Factory>(),
              std::make_unique<VertexBufferMock::Factory>(),
              std::make_unique<ElementBufferMock::Factory>(),
              std::make_unique<FrameBufferMock::Factory>(),
              std::make_unique<RenderBufferMock::Factory>(),
              std::make_unique<TextureMock::Factory>(), std::make_unique<CubemapMock::Factory>(),
              std::make_unique<ShaderMock::Factory>(), std::make_unique<ShaderCompilerMock>(),
              std::make_unique<RenderApiMock>()) {
        vertexArrayFactoryMock = static_cast<VertexArrayMock::Factory*>(vertexArrayFactory.get());
        vertexBufferFactoryMock =
            static_cast<VertexBufferMock::Factory*>(vertexBufferFactory.get());
        elementBufferFactoryMock =
            static_cast<ElementBufferMock::Factory*>(elementBufferFactory.get());
        frameBufferFactoryMock = static_cast<FrameBufferMock::Factory*>(frameBufferFactory.get());
        renderBufferFactoryMock =
            static_cast<RenderBufferMock::Factory*>(renderBufferFactory.get());
        textureFactoryMock = static_cast<TextureMock::Factory*>(textureFactory.get());
        cubemapFactoryMock = static_cast<CubemapMock::Factory*>(cubemapFactory.get());
        shaderFactoryMock  = static_cast<ShaderMock::Factory*>(shaderFactory.get());
        shaderCompilerMock = static_cast<ShaderCompilerMock*>(shaderCompiler.get());
        renderApiMock      = static_cast<RenderApiMock*>(renderApi.get());
    }

    // clang-format off
    inline static VertexArrayMock::Factory*   vertexArrayFactoryMock;
    inline static VertexBufferMock::Factory*  vertexBufferFactoryMock;
    inline static ElementBufferMock::Factory* elementBufferFactoryMock;
    inline static FrameBufferMock::Factory*   frameBufferFactoryMock;
    inline static RenderBufferMock::Factory*  renderBufferFactoryMock;
    inline static TextureMock::Factory*       textureFactoryMock;
    inline static CubemapMock::Factory*       cubemapFactoryMock;
    inline static ShaderMock::Factory*        shaderFactoryMock;
    inline static ShaderCompilerMock*         shaderCompilerMock;
    inline static RenderApiMock*              renderApiMock;
    // clang-format on
};