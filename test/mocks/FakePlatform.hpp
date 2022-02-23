#pragma once

#include "FakeGPU.hpp"
#include "FakeIO.hpp"
#include "ImageMock.hpp"
#include "ModelLoaderMock.hpp"
#include "sl/platform/Platform.hpp"

using namespace sl::platform;

auto createFakePlatform() {
    auto platform = Platform::Builder{}
                        .setIO<FakeIO>()
                        .setGPU<FakeGPU>()
                        .setImageFactory<ImageMock::Factory>()
                        .setModelLoader<ModelLoaderMock>()
                        .build();

    ModelLoaderMock::instance = static_cast<ModelLoaderMock*>(platform->modelLoader.get());
    ImageMock::factoryInstance = static_cast<ImageMock::Factory*>(platform->imageFactory.get());

    return platform;
}