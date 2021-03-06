#pragma once

#include "sl/gfx/buffer/FrameBuffer.h"

#include <gmock/gmock.h>

using namespace sl::gfx::buffer;

struct FrameBufferMock : public FrameBuffer {
    struct Factory : public FrameBuffer::Factory {
        MOCK_METHOD(std::shared_ptr<FrameBuffer>, create, (), (override));

        inline static Factory* instance = nullptr;
    };

    MOCK_METHOD(void, bindRenderBuffer, (sl::gfx::buffer::RenderBuffer&), (override));
    MOCK_METHOD(void, bindTexture, (sl::gfx::Texture&, unsigned int), (override));
    MOCK_METHOD(void, bindTexture, (sl::gfx::Texture&), (override));
    MOCK_METHOD(void, bindCubemap, (sl::gfx::Cubemap&), (override));
    MOCK_METHOD(void, bind, (), (override));
    MOCK_METHOD(void, unbind, (), (override));
    MOCK_METHOD(void, specifyColorBuffers, (const std::vector<unsigned int>&), (override));
};
