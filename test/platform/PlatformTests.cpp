#include <gtest/gtest.h>

#include "mocks/FakeGPU.hpp"
#include "mocks/FakeIO.hpp"
#include "mocks/ImageMock.hpp"
#include "mocks/ModelLoaderMock.hpp"
#include "sl/platform/Platform.hpp"

using namespace sl::platform;

TEST(PlatformTest, givenPlatformBuilder_whenCreatingWithoutAllRequiredComponentsSet_shouldThrow) {
    EXPECT_THROW(Platform::Builder{}.build(), std::runtime_error);
    EXPECT_THROW(Platform::Builder{}.setIO<FakeIO>().build(), std::runtime_error);
    EXPECT_THROW(Platform::Builder{}.setIO<FakeIO>().setGPU<FakeGPU>().build(), std::runtime_error);
    EXPECT_THROW(
        Platform::Builder{}
            .setIO<FakeIO>()
            .setGPU<FakeGPU>()
            .setImageFactory<ImageMock::Factory>()
            .build(),
        std::runtime_error
    );
}

TEST(PlatformTest, givenPlatformBuilder_whenCreatingWithAllRequiredComponents_shouldCreate) {
    auto platform = Platform::Builder{}
                        .setIO<FakeIO>()
                        .setGPU<FakeGPU>()
                        .setImageFactory<ImageMock::Factory>()
                        .setModelLoader<ModelLoaderMock>()
                        .build();
    EXPECT_NE(platform, nullptr);
}