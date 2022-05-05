#pragma once

#include <gmock/gmock.h>

#include <memory>

#include "sl/gpu/VertexBuffer.h"
#include "sl/gpu/fwd.h"

using namespace sl::gpu;

struct VertexBufferMock : public VertexBuffer {
   public:
    struct Factory : VertexBuffer::Factory {
        MOCK_METHOD(std::shared_ptr<VertexBuffer>, create, (void*, int, int), (override));
    };

    MOCK_METHOD(void, bind, (), (override));
    MOCK_METHOD(void, unbind, (), (override));

    MOCK_METHOD((unsigned int), getVerticesCount, (), (override));
};
