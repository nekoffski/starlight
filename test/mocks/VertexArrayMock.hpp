#pragma once

#include "sl/gfx/buffer/VertexArray.h"

#include <gmock/gmock.h>

using namespace sl::gfx;

struct VertexArrayMock : public buffer::VertexArray {
    struct Factory : buffer::VertexArray::Factory {
        MOCK_METHOD(std::shared_ptr<buffer::VertexArray>, create, (), (override));

        inline static Factory* instance = nullptr;
    };

    MOCK_METHOD((unsigned int), getVerticesCount, (), (override));
    MOCK_METHOD((unsigned int), getIndicesCount, (), (override));
    MOCK_METHOD(void, addVertexBuffer, (std::shared_ptr<sl::gfx::buffer::VertexBuffer>), (override));
    MOCK_METHOD(void, addElementBuffer, (std::shared_ptr<sl::gfx::buffer::ElementBuffer>), (override));
    MOCK_METHOD(void, bind, (), (override));
    MOCK_METHOD(void, unbind, (), (override));
};
