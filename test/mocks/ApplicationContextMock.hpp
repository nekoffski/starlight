#pragma once

#include "sl/app/ApplicationContext.h"

#include <gmock/gmock.h>

using namespace sl;
using namespace sl::app;

struct ApplicationContextMock : public ApplicationContext {
public:
    using ApplicationContext::ApplicationContext;

    MOCK_METHOD(void, onInit, (), (override));
    MOCK_METHOD(void, onAttach, (), (override));
    MOCK_METHOD(void, onDetach, (), (override));
    MOCK_METHOD(void, update, (app::SceneSystems & sceneSystems, float, float, core::Input&), (override));
    MOCK_METHOD(void, render, (gfx::Renderer&), (override));
    MOCK_METHOD(void, renderGui, (gui::GuiApi&), (override));
};