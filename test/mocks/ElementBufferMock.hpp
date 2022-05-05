#pragma once

#include <gmock/gmock.h>

#include "sl/gpu/ElementBuffer.h"

using namespace sl;
using namespace sl::gpu;

struct ElementBufferMock : public ElementBuffer {
    struct Factory : public ElementBuffer::Factory {
        MOCK_METHOD(std::shared_ptr<ElementBuffer>, create, (void*, int, unsigned int), (override));
    };

    MOCK_METHOD(void, bind, (), (override));
    MOCK_METHOD(void, unbind, (), (override));

    MOCK_METHOD((unsigned int), getIndicesCount, (), (override));
};
