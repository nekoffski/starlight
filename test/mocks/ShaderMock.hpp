#pragma once

#include "sl/gfx/Shader.h"

#include <gmock/gmock.h>

using namespace sl::gfx;
using namespace sl;

struct ShaderMock : public Shader {
    struct Factory : public Shader::Factory {
        MOCK_METHOD(std::shared_ptr<Shader>, create, (const std::string&, const std::string&, const std::string&), (override));

        inline static Factory* instance = nullptr;
    };

    MOCK_METHOD(void, enable, (), (override));
    MOCK_METHOD(void, disable, (), (override));
    MOCK_METHOD(void, setUniform, (const std::string&, float), (override));
    MOCK_METHOD(void, setUniform, (const std::string&, int), (override));
    MOCK_METHOD(void, setUniform, (const std::string&, unsigned int), (override));
    MOCK_METHOD(void, setUniform, (const std::string&, math::Mat4), (override));
    MOCK_METHOD(void, setUniform, (const std::string&, math::Mat3), (override));
    MOCK_METHOD(void, setUniform, (const std::string&, math::Vec4), (override));
    MOCK_METHOD(void, setUniform, (const std::string&, math::Vec3), (override));
    MOCK_METHOD(std::string, getVertexShaderPath, (), (override));
    MOCK_METHOD(std::string, getFragmentShaderPath, (), (override));
    MOCK_METHOD(std::string, getGeometryShaderPath, (), (override));
};
