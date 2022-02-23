#pragma once

#include <gmock/gmock.h>

#include "sl/gfx/FrameBuffer.h"

using namespace sl::gfx;

struct FrameBufferMock : public FrameBuffer {
    struct Factory : public FrameBuffer::Factory {
        MOCK_METHOD(std::shared_ptr<FrameBuffer>, create, (), (override));
    };

    MOCK_METHOD(void, bindRenderBuffer, (RenderBuffer&), (override));
    MOCK_METHOD(void, bindTexture, (Texture&, unsigned int), (override));
    MOCK_METHOD(void, bindTexture, (Texture&), (override));
    MOCK_METHOD(void, bindCubemap, (Cubemap&), (override));
    MOCK_METHOD(void, bind, (), (override));
    MOCK_METHOD(void, unbind, (), (override));
    MOCK_METHOD(void, specifyColorBuffers, (const std::vector<unsigned int>&), (override));
};
