#pragma once

#include "sl/platform/GPU.h"

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

using namespace sl;

struct FakeGPU : platform::GPU {
    FakeGPU()
        : GPU(
              std::make_unique<VertexArrayMock::Factory>(),
              std::make_unique<VertexBufferMock::Factory>(),
              std::make_unique<ElementBufferMock::Factory>(),
              std::make_unique<FrameBufferMock::Factory>(),
              std::make_unique<RenderBufferMock::Factory>(),
              std::make_unique<TextureMock::Factory>(),
              std::make_unique<CubemapMock::Factory>(),
              std::make_unique<ShaderMock::Factory>(),
              std::make_unique<ShaderCompilerMock>(),
              std::make_unique<RenderApiMock>()) {
    }
};