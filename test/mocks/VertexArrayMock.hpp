#pragma once

#include "sl/gfx/VertexArray.h"

#include <gmock/gmock.h>

using namespace sl::gfx;

struct VertexArrayMock : public VertexArray {
    struct Factory : VertexArray::Factory {
        MOCK_METHOD(std::shared_ptr<VertexArray>, create, (), (override));
    };

    MOCK_METHOD((unsigned int), getVerticesCount, (), (override));
    MOCK_METHOD((unsigned int), getIndicesCount, (), (override));
    MOCK_METHOD(void, addVertexBuffer, (std::shared_ptr<VertexBuffer>), (override));
    MOCK_METHOD(void, addElementBuffer, (std::shared_ptr<ElementBuffer>), (override));
    MOCK_METHOD(void, bind, (), (override));
    MOCK_METHOD(void, unbind, (), (override));
};
