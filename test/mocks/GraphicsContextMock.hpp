#pragma once

#include "sl/graphics/GraphicsContext.h"

#include <gmock/gmock.h>

using namespace sl::graphics;

struct GraphicsContextMock : public GraphicsContext {
    struct Factory : public GraphicsContext::Factory {
        MOCK_METHOD(std::unique_ptr<GraphicsContext>, create, (sl::core::types::NotNullPtr<void>), (override));
    };

    MOCK_METHOD(void, swapBuffers, (), (override));
    MOCK_METHOD(void, setViewport, (ViewFrustum::Viewport), (override));
    MOCK_METHOD(void, clearBuffers, (unsigned), (override));
};