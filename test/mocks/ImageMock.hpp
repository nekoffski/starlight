#pragma once

#include <gmock/gmock.h>

#include "sl/gfx/Image.h"

using namespace sl::gfx;

struct ImageMock : Image {
    struct Factory : Image::Factory {
        MOCK_METHOD(std::unique_ptr<Image>, create, (const std::string& path, int desiredChannels));
    };

    inline static ImageMock::Factory* factoryInstance = nullptr;

    MOCK_METHOD(unsigned char*, getBuffer, (), (const));
    MOCK_METHOD(sl::math::Size2D, getSize, (), (const));
    MOCK_METHOD(int, getChannelsCount, (), (const));
};