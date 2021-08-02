#pragma once

#include "sl/gfx/Texture.h"

#include <gmock/gmock.h>

using namespace sl::gfx;

struct TextureMock : Texture {
    struct Factory : Texture::Factory {
        MOCK_METHOD(std::unique_ptr<Texture>, create, (sl::gfx::Image&));
        MOCK_METHOD(std::unique_ptr<Texture>, create, (unsigned int, unsigned int, int, int));
    };

    MOCK_METHOD(void, bind, (unsigned int));
    MOCK_METHOD(void, unbind, ());
    MOCK_METHOD(unsigned int, getWidth, (), (const));
    MOCK_METHOD(unsigned int, getHeight, (), (const));
    MOCK_METHOD(unsigned int, getBuffer, (), (const));
};