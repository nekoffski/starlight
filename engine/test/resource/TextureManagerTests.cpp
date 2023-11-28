#include <starlight/resource/managers/TextureManager.h>
#include <starlight/renderer/ResourcePools.h>

#include "mock/All.h"

#include <gtest/gtest.h>

using namespace testing;
using namespace sl;
using namespace sl;

// TODO:: fix
struct TextureLoaderTests : public Test {
    RendererBackendMock backend;
    ResourcePoolsMock resourcePools;

    // TextureManager textureManager{ resourcePools };
    // std::unique_ptr<Texture> texture = std::make_unique<Texture>();
};

TEST_F(
  TextureLoaderTests, givenNonExistingTextureName_whenAcquiring_shouldReturnNullptr
) {
    // EXPECT_EQ(textureManager.acquire("bleble"), nullptr);
}

TEST_F(TextureLoaderTests, whenLoadingTexture_shouldLoadAndReturnPointer) {
    // auto pointer = texture.get();
    // EXPECT_CALL(backend, load(_, _))
    //   .Times(1)
    //   .WillOnce(Return(ByMove(std::move(texture))));

    // auto texture = textureManager.load("bleble");
    // EXPECT_EQ(pointer, texture);
}

TEST_F(
  TextureLoaderTests,
  givenLoadedTexture_whenLoadingTheSameTexture_shouldReturnPointer
) {
    // auto pointer = texture.get();
    // EXPECT_CALL(textureLoader, load(_, _, _))
    //   .Times(1)
    //   .WillOnce(Return(ByMove(std::move(texture))));

    // EXPECT_EQ(pointer, textureManager.load("bleble"));
    // EXPECT_EQ(pointer, textureManager.load("bleble"));
}

TEST_F(
  TextureLoaderTests, givenLoadedTexture_whenDestroyingTexture_shouldDestroyTexture
) {
    //     auto pointer = texture.get();
    //     EXPECT_CALL(textureLoader, load(_, _, _))
    //       .Times(1)
    //       .WillOnce(Return(ByMove(std::move(texture))));

    //     ASSERT_EQ(pointer, textureManager.load("bleble"));
    //     textureManager.destroy("bleble");

    //     EXPECT_EQ(textureManager.acquire("bleble"), nullptr);
}

TEST_F(
  TextureLoaderTests,
  givenLoadedTexture_whenDestroyingAllTextures_shouldDestroyTexture
) {
    // auto pointer = texture.get();
    // EXPECT_CALL(textureLoader, load(_, _, _))
    //   .Times(1)
    //   .WillOnce(Return(ByMove(std::move(texture))));

    // ASSERT_EQ(pointer, textureManager.load("bleble"));
    // textureManager.destroyAll();

    // EXPECT_EQ(textureManager.acquire("bleble"), nullptr);
}

TEST_F(TextureLoaderTests, whenDestroyingNotExistingTexture_shouldNotThrow) {
    // EXPECT_NO_THROW({ textureManager.destroy("bleble"); });
}