#pragma once

#include "sl/graphics/ShaderCompilerImpl.h"

#include <gmock/gmock.h>

using namespace sl::graphics;

struct ShaderCompilerImplMock : public ShaderCompilerImpl {
    struct Factory : public ShaderCompilerImpl::Factory {
        MOCK_METHOD(std::unique_ptr<ShaderCompilerImpl>, create, (), (override));
    };

    MOCK_METHOD(void, compile, (std::shared_ptr<Shader>&), (override));
};
