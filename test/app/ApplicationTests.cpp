#include "sl/app/Application.h"

#include <gtest/gtest.h>

#include "mocks/ApplicationContextMock.hpp"
#include "mocks/Mocks.hpp"

using namespace testing;

struct ApplicationTests : Test {
    void SetUp() override {
        setupMockFactories();
    }

    void TearDown() override {
        resetMockFactories();
    }

    void setupInitExpects() {
        EXPECT_CALL(*mocks.windowMock, init()).Times(1);
        EXPECT_CALL(*mocks.windowMock, setResizeCallback(_)).Times(1);

        EXPECT_CALL(*FrameBufferMock::Factory::instance, create()).WillRepeatedly(Return(mocks.frameBufferMock));
        EXPECT_CALL(*VertexArrayMock::Factory::instance, create()).WillRepeatedly(Return(mocks.vertexArrayMock));
        EXPECT_CALL(*ElementBufferMock::Factory::instance, create(_, _, _)).WillRepeatedly(Return(mocks.elementBufferMock));
        EXPECT_CALL(*VertexBufferMock::Factory::instance, create(_, _, _)).WillRepeatedly(Return(mocks.vertexBufferMock));
        EXPECT_CALL(*ShaderMock::Factory::instance, create(_, _, _)).WillRepeatedly(Return(mocks.shaderMock));

        int someValue = 5;

        EXPECT_CALL(*mocks.windowMock, getHandle()).Times(1).WillOnce(Return(&someValue));

        EXPECT_CALL(*WindowMock::Factory::instance, create(_, _)).Times(1).WillOnce(Return(mocks.windowMock));
        EXPECT_CALL(*GraphicsContextMock::Factory::instance, create(_)).Times(1).WillOnce(Return(mocks.graphicsContextMock));
        EXPECT_CALL(*RenderApiMock::Factory::instance, create()).Times(1).WillOnce(Return(ByMove(std::move(mocks.renderApiMock))));
        EXPECT_CALL(*ShaderCompilerImplMock::Factory::instance, create()).Times(1).WillOnce(Return(ByMove(std::move(mocks.shaderCompilerImplMock))));
        EXPECT_CALL(*ModelLoaderImplMock::Factory::instance, create()).Times(1).WillOnce(Return(ByMove(std::move(mocks.modelLoaderImplMock))));
        EXPECT_CALL(*GuiApiMock::Factory::instance, create(_)).Times(1).WillOnce(Return(mocks.guiApiMock));
    }

    Mocks mocks;
    std::shared_ptr<Application> application = std::make_shared<app::Application>();
};

TEST_F(ApplicationTests, givenApplication_whenCallingInit_shouldCallSeveralFactories) {
    setupInitExpects();

    application->init();
}

TEST_F(ApplicationTests, givenApplication_whenCreatingContext_shouldCreateContext) {
    auto application = std::make_shared<app::Application>();
    auto context = application->createContext<ApplicationContextMock>("ident");

    EXPECT_TRUE(context != nullptr);
    EXPECT_TRUE(dynamic_cast<ApplicationContextMock*>(context.get()) != nullptr);
}

TEST_F(ApplicationTests, givenApplication_whenSwitchingContext_shouldCallAttachMethods) {
    auto context1 = application->createContext<ApplicationContextMock>("ident1");
    auto context2 = application->createContext<ApplicationContextMock>("ident2");

    EXPECT_CALL(*context1, onAttach()).Times(1);
    EXPECT_CALL(*context2, onAttach()).Times(1);
    EXPECT_CALL(*context1, onDetach()).Times(1);
    EXPECT_CALL(*context2, onDetach()).Times(0);

    application->switchContext(context1);
    application->switchContext(context2);
}

TEST_F(ApplicationTests, givenApplication_whenGettingActiveContext_shouldReturnActiveContext) {
    auto context1 = application->createContext<ApplicationContextMock>("ident1");
    EXPECT_EQ(application->getActiveContext(), nullptr);

    application->switchContext(context1);
    EXPECT_EQ(context1.get(), application->getActiveContext().get());
}

TEST_F(ApplicationTests, givenApplication_whenUpdating_shouldUpdateSubComponents) {
    setupInitExpects();
    application->init();

    auto context = application->createContext<ApplicationContextMock>("ident");
    application->switchContext(context);

    EXPECT_CALL(*mocks.windowMock, update(_)).Times(1);
    EXPECT_CALL(*context, update(_, _, _)).Times(1);

    bool isMouseButtonPressed = true;
    bool cursorState;

    EXPECT_CALL(*mocks.windowMock, changeCursorState(_)).Times(1).WillOnce(SaveArg<0>(&cursorState));

    application->update(1.0f, 1.0f);

    EXPECT_EQ(!isMouseButtonPressed, cursorState);
}

TEST_F(ApplicationTests, givenApplication_whenRendering_shouldRenderSubComponents) {
    setupInitExpects();
    application->init();

    auto context = application->createContext<ApplicationContextMock>("ident");
    application->switchContext(context);

    EXPECT_CALL(*mocks.graphicsContextMock, clearBuffers(_)).Times(1);
    EXPECT_CALL(*mocks.graphicsContextMock, swapBuffers()).Times(1);
    EXPECT_CALL(*mocks.guiApiMock, begin()).Times(1);
    EXPECT_CALL(*mocks.guiApiMock, end()).Times(1);
    EXPECT_CALL(*context, render(_)).Times(1);
    EXPECT_CALL(*context, renderGui(_)).Times(1);

    application->render();
}

TEST_F(ApplicationTests, givenApplication_whenCallingForceStop_shouldCallWindowShouldClose) {
    setupInitExpects();
    application->init();

    bool arg;
    EXPECT_CALL(*mocks.windowMock, setShouldClose(_)).Times(1).WillOnce(SaveArg<0>(&arg));

    application->forceStop();

    EXPECT_TRUE(arg);
}

TEST_F(ApplicationTests, givenApplication_whenCallingIsRunning_shouldReturnNegationOfWindowShouldClose) {
    setupInitExpects();
    application->init();

    EXPECT_CALL(*mocks.windowMock, getShouldClose()).Times(2).WillOnce(Return(true)).WillOnce(Return(false));

    EXPECT_FALSE(application->isRunning());
    EXPECT_TRUE(application->isRunning());
}