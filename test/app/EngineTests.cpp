#include "sl/app/Engine.h"

#include <gtest/gtest.h>

#include "mocks/FakePlatform.hpp"

using namespace sl;
using namespace sl::app;

using namespace testing;

struct EngineTests : Test {
    std::unique_ptr<platform::Platform> fakePlatform = createFakePlatform();
    cfg::Config config {};
};

TEST_F(EngineTests, givenEngineBuilderWithoutFieldsSet_whenCreating_shouldThrow) {
    EXPECT_THROW(Engine::Builder {}.build(), std::runtime_error);
    EXPECT_THROW(Engine::Builder {}.setPlatform(fakePlatform.get()).build(), std::runtime_error);
    EXPECT_THROW(Engine::Builder {}.setConfig(&config).build(), std::runtime_error);
}

TEST_F(EngineTests, givenEngineBuilderWithFieldsSet_whenCreating_shouldNotThrowAndInitSystems) {
    EXPECT_CALL(*FakeIO::windowMock, init()).Times(1);
    EXPECT_CALL(*FakeIO::windowMock, makeContextCurrent()).Times(1);
    EXPECT_CALL(*FakeIO::windowMock, setResizeCallback(_)).Times(1);
    EXPECT_CALL(*FakeIO::windowMock, getHandle()).Times(1);

    EXPECT_CALL(*FakeIO::inputMock, init(_)).Times(1);

    EXPECT_CALL(*FakeGPU::renderApiMock, init()).Times(1);

    EXPECT_NO_THROW(
        EXPECT_NE(
            Engine::Builder {}.setConfig(&config).setPlatform(fakePlatform.get()).build(), nullptr));
}
