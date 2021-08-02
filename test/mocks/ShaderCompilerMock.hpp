#pragma once

#include "sl/gfx/ShaderCompiler.h"

#include <gmock/gmock.h>

using namespace sl::gfx;

struct ShaderCompilerMock : public ShaderCompiler {
    MOCK_METHOD(void, compile, (Shader&), (override));
};
