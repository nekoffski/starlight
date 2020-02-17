#include <memory>

#include <gtest/gtest.h>

#include <starlight/core/event/EventBus.h>
#include <starlight/core/event/EventDispatcher.h>

using namespace starl::core;

class EventTests : public testing::Test {
protected:
    void SetUp() override {
        m_eventBus = std::make_unique<event::EventBus>();
    }

    std::unique_ptr<event::EventBus> m_eventBus;
};

struct Event1 : event::Event {
    using event::Event::Event;
};

struct Event2 : event::Event {
    using event::Event::Event;

};

class SomeService : public event::IEventObserver {
public:
    void onEvent(std::shared_ptr<event::Event> e) override {}
};

TEST_F(EventTests, givenSameIdentTwice_whenRegisteringObserver_shouldReturnError) {
    SomeService a, b;
    auto ident = "ident";

    auto err1 = m_eventBus->registerObserver(&a, ident);
    auto err2 = m_eventBus->registerObserver(&b, ident);

    EXPECT_FALSE(err1);
    EXPECT_TRUE(err2);
    EXPECT_EQ(err2.code, static_cast<int>(event::ErrorCode::TWO_OBSERVERS_WITH_SAME_IDENT));
    EXPECT_EQ(err2.severity, err::Severity::FATAL);
}

TEST_F(EventTests, givenEvent_whenCreatingDefault_expectIsHandledAndDispatchOnceSetToFalse) {
    auto ev = Event1{};

    EXPECT_FALSE(ev.dispatchOnce);
    EXPECT_FALSE(ev.isHandled);
}

TEST_F(EventTests, givenEvent_whenDispatching_expectInvokingProperCallback) {
    auto event = std::make_shared<Event1>();
    auto dispatcher = event::EventDispatcher(event);

    bool firstInvoked = false;
    bool secondInvoked = false;

    auto cb1 = [&firstInvoked](std::shared_ptr<Event1> e) -> bool {
        firstInvoked = true;
        return true;
    };

    auto cb2 = [&secondInvoked](std::shared_ptr<Event2> e) -> bool {
        secondInvoked = true;
        return true;
    };

    dispatcher.setCallback<Event1>(cb1);

    EXPECT_TRUE(firstInvoked);
    EXPECT_FALSE(secondInvoked);
};
