#pragma once

#include "sl/gfx/buffer/ElementBuffer.h"

#include <gmock/gmock.h>

namespace sl::gfx::buffer {

struct ElementBufferMock : public ElementBuffer {
    struct Factory : public ElementBuffer::Factory {
        MOCK_METHOD(std::shared_ptr<ElementBuffer>, create, (core::types::NotNullPtr<void>, int, unsigned int), (override));

        inline static Factory* instance = nullptr;
    };

    MOCK_METHOD(void, bind, (), (override));
    MOCK_METHOD(void, unbind, (), (override));

    MOCK_METHOD((unsigned int), getIndicesCount, (), (override));
};
}
