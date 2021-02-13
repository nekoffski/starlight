#include <gtest/gtest.h>

#include <jsoncpp/json/value.h>
#include <memory>

#include "sl/application/Serializer.h"
#include "sl/asset/AssetManager.h"
#include "sl/core/Json.h"
#include "sl/scene/Scene.h"

#include "mocks/FileSystemMock.hpp"

using namespace testing;

using namespace sl::core;
using namespace sl::application;
using namespace sl::asset;
using namespace sl::scene;

namespace {

class SerializerTests : public Test {
protected:
	std::shared_ptr<FileSystemMock> m_fsMock = std::make_shared<FileSystemMock>();

    const std::string m_filename = "exampleFilename";
    const std::string m_path = "/path/";

    std::shared_ptr<Scene> m_scene = std::make_shared<Scene>();
    AssetManager m_assetManager;

    Serializer m_serializer = Serializer{ m_path, m_filename, m_fsMock };

    void prevalidateSerializerJson(Json::Value root) {
        ASSERT_EQ(root.size(), 2);
        ASSERT_TRUE(root.isMember("assets"));
        ASSERT_TRUE(root.isMember("scene"));
        ASSERT_TRUE(root["scene"].isMember("entities"));
    }
};

TEST_F(SerializerTests, whenCreatingSerializerWithDefaultFileSystem_shouldBeCreatedWell) {
    Serializer serializer{ m_path, m_filename };
}

TEST_F(SerializerTests, givenEmptySceneAndEmptyAssetManager_whenSerializing_shouldReturnEmptyAssetsAndEntities) {
    EXPECT_CALL(*m_fsMock, writeFile(_, _)).Times(1).WillOnce(Invoke(writeFileArgsHijacker));

    m_serializer.serialize(m_assetManager, m_scene);

    auto json = parseJson(FileSystemMock::capturedFileContent);
    prevalidateSerializerJson(json);
    EXPECT_EQ(json["assets"].size(), 0);
    EXPECT_EQ(json["scene"]["entities"].size(), 0);
}
}
