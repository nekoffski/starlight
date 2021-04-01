#pragma once

#include <memory>

#include <gmock/gmock.h>

#include "sl/gfx/buffer/VertexBuffer.h"
#include "sl/gfx/fwd.h"

using namespace sl::gfx::buffer;

struct VertexBufferMock : public VertexBuffer {
public:
    struct Factory : buffer::VertexBuffer::Factory {
        MOCK_METHOD(std::shared_ptr<VertexBuffer>, create, (sl::core::NotNullPtr<void>, int, int), (override));

        inline static Factory* instance = nullptr;
    };

    MOCK_METHOD(void, bind, (), (override));
    MOCK_METHOD(void, unbind, (), (override));

    MOCK_METHOD((unsigned int), getVerticesCount, (), (override));
};
