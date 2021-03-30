#include "sl/app/Application.h"

#include <gtest/gtest.h>

#include "mocks/Mocks.hpp"

using namespace testing;

struct ApplicationTests : Test {
    void SetUp() override {
        setupMockFactories();
    }

    void TearDown() override {
        resetMockFactories();
    }

    Mocks mocks;
};

TEST_F(ApplicationTests, givenApplication_whenCallingInit_shouldCallSeveralFactories) {
    EXPECT_CALL(*mocks.windowMock, init()).Times(1);
    EXPECT_CALL(*mocks.windowMock, setResizeCallback(_)).Times(1);

    EXPECT_CALL(*FrameBufferMock::Factory::instance, create()).WillRepeatedly(Return(mocks.frameBufferMock));
    EXPECT_CALL(*VertexArrayMock::Factory::instance, create()).WillRepeatedly(Return(mocks.vertexArrayMock));
    EXPECT_CALL(*ElementBufferMock::Factory::instance, create(_, _, _)).WillRepeatedly(Return(mocks.elementBufferMock));
    EXPECT_CALL(*VertexBufferMock::Factory::instance, create(_, _, _)).WillRepeatedly(Return(mocks.vertexBufferMock));
    EXPECT_CALL(*ShaderMock::Factory::instance, create(_, _, _)).WillRepeatedly(Return(mocks.shaderMock));

    EXPECT_CALL(*WindowMock::Factory::instance, create(_, _)).Times(1).WillOnce(Return(mocks.windowMock));
    EXPECT_CALL(*InputMock::Factory::instance, create(_)).Times(1).WillOnce(Return(mocks.inputMock));
    EXPECT_CALL(*GraphicsContextMock::Factory::instance, create(_)).Times(1).WillOnce(Return(mocks.graphicsContextMock));
    EXPECT_CALL(*RenderApiMock::Factory::instance, create()).Times(1).WillOnce(Return(ByMove(std::move(mocks.renderApiMock))));
    EXPECT_CALL(*ShaderCompilerImplMock::Factory::instance, create()).Times(1).WillOnce(Return(ByMove(std::move(mocks.shaderCompilerImplMock))));
    EXPECT_CALL(*ModelLoaderImplMock::Factory::instance, create()).Times(1).WillOnce(Return(ByMove(std::move(mocks.modelLoaderImplMock))));

    int someValue = 5;

    EXPECT_CALL(*mocks.windowMock, getHandle()).Times(1).WillOnce(Return(&someValue));

    auto application = std::make_shared<app::Application>();
    application->init();
}
