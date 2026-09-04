#include "starlight/event/EventSystem.hh"

#include <gtest/gtest.h>

#include <chrono>
#include <future>
#include <memory>

namespace {

struct Ping {
    int value;
};

struct Pong {};

TEST(EventSystem, DeliversQueuedEventsWhenDispatched) {
    sl::EventSystem eventSystem;
    auto bus = eventSystem.createBus();
    int received = 0;

    bus.on<Ping>([&received](const Ping& event) { received = event.value; });
    bus.post<Ping>(42);

    EXPECT_EQ(received, 0);

    eventSystem.dispatch();

    EXPECT_EQ(received, 42);
}

TEST(EventSystem, DefersEventsPostedDuringDispatchUntilTheNextDispatch) {
    sl::EventSystem eventSystem;
    auto bus = eventSystem.createBus();
    int pings = 0;
    int pongs = 0;

    bus.on<Ping>([&](const Ping&) {
        ++pings;
        bus.post<Pong>();
    });
    bus.on<Pong>([&pongs](const Pong&) { ++pongs; });
    bus.post<Ping>(1);

    eventSystem.dispatch();

    EXPECT_EQ(pings, 1);
    EXPECT_EQ(pongs, 0);

    eventSystem.dispatch();

    EXPECT_EQ(pongs, 1);
}

TEST(EventSystem, BlocksNewSubscriptionsUntilDispatchCompletes) {
    sl::EventSystem eventSystem;
    auto subscriber = eventSystem.createBus();
    auto registeringBus = eventSystem.createBus();
    std::promise<void> callbackStarted;
    std::promise<void> finishDispatch;
    const auto canFinish = finishDispatch.get_future().share();

    subscriber.on<Ping>([&](const Ping&) {
        callbackStarted.set_value();
        canFinish.wait();
    });
    subscriber.post<Ping>(1);

    auto dispatch =
        std::async(std::launch::async, [&] { eventSystem.dispatch(); });
    callbackStarted.get_future().wait();

    auto registration = std::async(std::launch::async, [&] {
        registeringBus.on<Ping>([](const Ping&) {});
    });

    EXPECT_EQ(
        registration.wait_for(std::chrono::milliseconds{100}),
        std::future_status::timeout
    );

    finishDispatch.set_value();
    dispatch.get();

    EXPECT_EQ(
        registration.wait_for(std::chrono::seconds{1}),
        std::future_status::ready
    );
    registration.get();
}

TEST(EventSystem, RemovesDestroyedBusSubscriptions) {
    sl::EventSystem eventSystem;
    auto publisher = eventSystem.createBus();
    int calls = 0;

    {
        auto subscriber = eventSystem.createBus();
        subscriber.on<Ping>([&calls](const Ping&) { ++calls; });
    }

    publisher.post<Ping>(1);
    eventSystem.dispatch();

    EXPECT_EQ(calls, 0);
}

TEST(EventSystem, AcceptsMoveOnlyCallbacks) {
    sl::EventSystem eventSystem;
    auto bus = eventSystem.createBus();
    auto calls = std::make_unique<int>(0);
    int* callsView = calls.get();

    bus.on<Ping>([calls = std::move(calls)](const Ping&) { ++*calls; });
    bus.post<Ping>(1);
    eventSystem.dispatch();

    EXPECT_EQ(*callsView, 1);
}

}  // namespace
