#pragma once

#include <gmock/gmock.h>

#include "sl/gfx/ShaderCompiler.h"

using namespace sl::gfx;

struct ShaderCompilerMock : public ShaderCompiler {
    MOCK_METHOD(void, compile, (Shader&), (override));
};
