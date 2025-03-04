#include <gtest/gtest.h>

#include "starlight/app/scene2/Scene.hh"

using namespace sl;

struct SceneTests : testing::Test {
    v2::Scene scene;
};

TEST_F(SceneTests, givenScene_whenSettingSkybox_shouldReturnRenderPacketWithSkybox) {
    ASSERT_EQ(scene.assembleRenderPacket().skybox, nullptr);

    auto skybox = SharedPtr<Skybox>::create(nullptr);
    scene.setSkybox(skybox);
    ASSERT_EQ(scene.assembleRenderPacket().skybox, skybox.get());

    scene.resetSkybox();
    ASSERT_EQ(scene.assembleRenderPacket().skybox, nullptr);
}
