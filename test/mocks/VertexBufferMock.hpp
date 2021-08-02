#pragma once

#include <memory>

#include <gmock/gmock.h>

#include "sl/gfx/VertexBuffer.h"
#include "sl/gfx/fwd.h"

using namespace sl::gfx;

struct VertexBufferMock : public VertexBuffer {
public:
    struct Factory : VertexBuffer::Factory {
        MOCK_METHOD(std::shared_ptr<VertexBuffer>, create, (sl::core::NotNullPtr<void>, int, int), (override));
    };

    MOCK_METHOD(void, bind, (), (override));
    MOCK_METHOD(void, unbind, (), (override));

    MOCK_METHOD((unsigned int), getVerticesCount, (), (override));
};
