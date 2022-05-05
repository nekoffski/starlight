#pragma once

#include <gmock/gmock.h>

#include "sl/gpu/Texture.h"

using namespace sl::gpu;

struct TextureMock : Texture {
    struct Factory : Texture::Factory {
        MOCK_METHOD(std::unique_ptr<Texture>, create, (sl::gpu::Image&));
        MOCK_METHOD(std::unique_ptr<Texture>, create, (unsigned int, unsigned int, int, int));
    };

    MOCK_METHOD(void, bind, (unsigned int));
    MOCK_METHOD(void, unbind, ());
    MOCK_METHOD(unsigned int, getWidth, (), (const));
    MOCK_METHOD(unsigned int, getHeight, (), (const));
    MOCK_METHOD(unsigned int, getBuffer, (), (const));
};