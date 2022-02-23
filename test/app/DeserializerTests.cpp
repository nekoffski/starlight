#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <kc/json/Json.h>

#include <memory>

#include "mocks/FileSystemMock.hpp"
#include "sl/app/Deserializer.h"
#include "sl/asset/AssetManager.h"
#include "sl/core/Errors.hpp"
#include "sl/scene/Scene.h"

using namespace testing;

using namespace sl::core;
using namespace sl::app;
using namespace sl::asset;
using namespace sl::scene;
using namespace kc::json;

namespace {

class DeserializerTests : public Test {
   protected:
    std::shared_ptr<FileSystemMock> m_fsMock = std::make_shared<FileSystemMock>();

    const std::string m_filename = "exampleFilename.starlscene";

    Scene m_scene;
    AssetManager m_assetManager;

    Deserializer deserializer = Deserializer{m_assetManager, &m_scene};
};

TEST_F(DeserializerTests, whenFileDoesNotExists_shouldThrow) {
    EXPECT_CALL(*m_fsMock, isFile(_)).Times(1).WillOnce(Return(false));

    EXPECT_THROW(deserializer.deserialize(m_filename, *m_fsMock), kc::core::ErrorBase);
}

class DeserializerTestsInvalidJson : public DeserializerTests,
                                     public WithParamInterface<std::string> {};

TEST_P(DeserializerTestsInvalidJson, givenInvalidJson_whenDeserializing_shouldThrow) {
    EXPECT_CALL(*m_fsMock, isFile(_)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*m_fsMock, readFile(_)).Times(1).WillOnce(Return(GetParam()));

    EXPECT_THROW(deserializer.deserialize(m_filename, *m_fsMock), kc::core::ErrorBase);
};

static const std::vector<std::string> invalidJsonTestData = {
    "not a json definitely {",
    "{}",
    "{\"scene:{}\"}",
    "{\"assets\": {}}",
};

INSTANTIATE_TEST_CASE_P(_, DeserializerTestsInvalidJson, ::testing::ValuesIn(invalidJsonTestData));
}  // namespace
