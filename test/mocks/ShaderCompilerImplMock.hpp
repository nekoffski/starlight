#pragma once

#include "sl/gfx/ShaderCompilerImpl.h"

#include <gmock/gmock.h>

using namespace sl::gfx;

struct ShaderCompilerImplMock : public ShaderCompilerImpl {
    struct Factory : public ShaderCompilerImpl::Factory {
        MOCK_METHOD(std::unique_ptr<ShaderCompilerImpl>, create, (), (override));

        inline static Factory* instance = nullptr;
    };

    MOCK_METHOD(void, compile, (std::shared_ptr<Shader>&), (override));
};
