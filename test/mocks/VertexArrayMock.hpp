#pragma once

#include <gmock/gmock.h>

#include "sl/gpu/VertexArray.h"

using namespace sl::gpu;

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
