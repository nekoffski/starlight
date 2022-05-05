#pragma once

#include <gmock/gmock.h>

#include "sl/gpu/ShaderCompiler.h"

using namespace sl::gpu;

struct ShaderCompilerMock : public ShaderCompiler {
    MOCK_METHOD(void, compile, (Shader&), (override));
};
