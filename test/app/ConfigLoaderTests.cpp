#include "sl/app/ConfigLoader.h"

#include <gtest/gtest.h>

#include "mocks/FileSystemMock.hpp"
#include "sl/core/Errors.hpp"

using namespace sl;
using namespace sl::app;
using namespace sl::core;
using namespace kc::json;
using namespace testing;

namespace {

struct ConfigLoaderTests : testing::Test {
    FileSystemMock fileSystemMock;
};

TEST_F(ConfigLoaderTests, whenFileDoesNotExists_shouldThrowError) {
    EXPECT_CALL(fileSystemMock, isFile(_)).Times(1).WillOnce(Return(false));

    EXPECT_THROW(ConfigLoader {}.loadFromFile("", fileSystemMock), core::ConfigError);
}

TEST_F(ConfigLoaderTests, givenCorruptedJson_whenLoadingConfig_shouldThrow) {
    EXPECT_CALL(fileSystemMock, isFile(_)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(fileSystemMock, readFile(_)).Times(1).WillOnce(Return("}not a json at all{"));

    EXPECT_THROW(ConfigLoader {}.loadFromFile("", fileSystemMock), core::JsonError);
}

struct ConfigLoaderInvalidConfigTests : ConfigLoaderTests, WithParamInterface<std::string> {
};

const std::vector<std::string> invalidConfigTestCases = {
    JsonBuilder {}.asString(),
    JsonBuilder {}.beginObject("paths").endObject().asString(),
    JsonBuilder {}.beginObject("paths").addField("shaders", "abcd"s).endObject().asString(),
    JsonBuilder {}.beginObject("paths").addField("textures", "abcd"s).endObject().asString(),
    JsonBuilder {}.beginObject("paths").addField("models", "abcd"s).endObject().asString(),
    JsonBuilder {}.beginObject("paths").addField("models", "abcd"s).addField("shaders", "abcd"s).endObject().asString(),
    JsonBuilder {}.beginObject("paths").addField("textures", "abcd"s).addField("shaders", "abcd"s).endObject().asString(),
    JsonBuilder {}.beginObject("paths").addField("textures", "abcd"s).addField("models", "abcd"s).endObject().asString(),
};

TEST_P(ConfigLoaderInvalidConfigTests, givenInvalidConfig_whenLoadingConfig_shouldThrow) {
    auto rawJsonConfig = GetParam();

    EXPECT_CALL(fileSystemMock, isFile(_)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(fileSystemMock, readFile(_)).Times(1).WillOnce(Return(rawJsonConfig));

    EXPECT_THROW(ConfigLoader {}.loadFromFile("", fileSystemMock), core::ConfigError);
}

INSTANTIATE_TEST_CASE_P(_, ConfigLoaderInvalidConfigTests, ValuesIn(invalidConfigTestCases));

}