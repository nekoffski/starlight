#pragma once

#include "sl/gfx/RenderBuffer.h"

#include <gmock/gmock.h>

using namespace sl::gfx;

struct RenderBufferMock : RenderBuffer {
    struct Factory : RenderBuffer::Factory {
        MOCK_METHOD(std::unique_ptr<RenderBuffer>, create, (int format, unsigned int width, unsigned int height));
    };

    MOCK_METHOD(unsigned int, getBufferId, (), (const));
    MOCK_METHOD(void, bind, ());
    MOCK_METHOD(void, unbind, ());
};