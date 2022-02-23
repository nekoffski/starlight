#include <gtest/gtest.h>
#include <kc/json/Json.h>

#include <memory>

#include "mocks/FileSystemMock.hpp"
#include "sl/app/Serializer.h"
#include "sl/asset/AssetManager.h"
#include "sl/ecs/Entity.h"
#include "sl/glob/Globals.h"
#include "sl/scene/Scene.h"

using namespace testing;

using namespace kc::json;
using namespace sl::core;
using namespace sl::app;
using namespace sl::asset;
using namespace sl::scene;

namespace {

class SerializerTests : public Test {
   protected:
    std::shared_ptr<FileSystemMock> m_fsMock = std::make_shared<FileSystemMock>();

    const std::string m_filename = "/path/exampleFilename";

    sl::glob::Globals globals;

    Scene m_scene;
    AssetManager m_assetManager;

    Serializer m_serializer = Serializer{m_filename};

    void prevalidateSerializerJson(Json::Value root) {
        ASSERT_EQ(root.size(), 2);
        ASSERT_TRUE(root.isMember("assets"));
        ASSERT_TRUE(root.isMember("scene"));
        ASSERT_TRUE(root["scene"].isMember("entities"));
    }
};

TEST_F(SerializerTests, whenCreatingSerializerWithDefaultFileSystem_shouldBeCreatedWell) {
    Serializer serializer{m_filename};
}

TEST_F(SerializerTests,
       givenEmptySceneAndEmptyAssetManager_whenSerializing_shouldReturnEmptyAssetsAndEntities) {
    EXPECT_CALL(*m_fsMock, writeFile(_, _, _)).Times(1).WillOnce(Invoke(writeFileArgsHijacker));

    m_serializer.serialize(m_assetManager, &m_scene, *m_fsMock);

    auto json = loadJson(FileSystemMock::capturedFileContent);
    prevalidateSerializerJson(json);
    EXPECT_TRUE(json.isMember("assets"));
    EXPECT_EQ(json["assets"]["cubemaps"].size(), 0);

    EXPECT_EQ(json["scene"]["entities"].size(), 0);
}
}  // namespace
