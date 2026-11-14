#include <gtest/gtest.h>

#include "starlight/event/EventSystem.hh"
#include "starlight/event/Events.hh"
#include "starlight/platform/sdl/SDLWindow.hh"
#include "starlight/window/WindowEvents.hh"

namespace {

using namespace sl;

static_assert(std::is_base_of_v<Window, SDLWindow>);
static_assert(std::is_base_of_v<RenderSurfaceProvider, SDLWindow>);

class WindowInput : public testing::Test {
   protected:
    void SetUp() override {
        ASSERT_TRUE(SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "dummy"));
        window = std::make_unique<SDLWindowBase>(Config{}, events.createBus());
        window->pollEvents();
        events.dispatch();
    }

    void TearDown() override {
        window.reset();
        SDL_ResetHint(SDL_HINT_VIDEO_DRIVER);
    }

    void push(SDL_Event event) { ASSERT_TRUE(SDL_PushEvent(&event)); }

    void key(
        SDL_EventType type, SDL_Scancode scancode, bool repeat = false,
        SDL_WindowID target = 0
    ) {
        SDL_Event event{};
        event.type = type;
        event.key.windowID = target ? target : window->id().get();
        event.key.scancode = scancode;
        event.key.key = SDLK_Z;  // Translation uses physical position.
        event.key.mod = SDL_KMOD_LCTRL | SDL_KMOD_RSHIFT;
        event.key.repeat = repeat;
        event.key.down = type == SDL_EVENT_KEY_DOWN;
        push(event);
    }

    void button(SDL_EventType type, Uint8 button = SDL_BUTTON_LEFT) {
        SDL_Event event{};
        event.type = type;
        event.button.windowID = window->id().get();
        event.button.button = button;
        event.button.x = 10;
        event.button.y = 20;
        push(event);
    }

    EventSystem events;
    EventBus bus{events.createBus()};
    std::unique_ptr<SDLWindowBase> window;
};

TEST_F(WindowInput, DeliversShortTapsAndEventTimeModifiers) {
    int pressed = 0;
    int released = 0;
    bus.on<KeyPressedEvent>([&](const auto& event) {
        ++pressed;
        EXPECT_EQ(event.window, window->id());
        EXPECT_EQ(event.key, Key::w);
        EXPECT_TRUE(event.modifiers.control);
        EXPECT_TRUE(event.modifiers.shift);
        EXPECT_FALSE(event.modifiers.alt);
        EXPECT_FALSE(event.modifiers.meta);
        EXPECT_FALSE(event.repeat);
    });
    bus.on<KeyReleasedEvent>([&](const auto& event) {
        ++released;
        EXPECT_EQ(event.window, window->id());
        EXPECT_EQ(event.key, Key::w);
        EXPECT_TRUE(event.modifiers.control);
    });

    key(SDL_EVENT_KEY_DOWN, SDL_SCANCODE_W);
    key(SDL_EVENT_KEY_UP, SDL_SCANCODE_W);
    window->pollEvents();
    EXPECT_EQ(pressed, 0);
    EXPECT_EQ(released, 0);
    events.dispatch();
    EXPECT_EQ(pressed, 1);
    EXPECT_EQ(released, 1);
}

TEST_F(WindowInput, QueriesNativeStateRatherThanInjectedEvents) {
    key(SDL_EVENT_KEY_DOWN, SDL_SCANCODE_W);
    button(SDL_EVENT_MOUSE_BUTTON_DOWN);
    SDL_Event event{};
    event.type = SDL_EVENT_WINDOW_FOCUS_GAINED;
    event.window.windowID = window->id().get();
    push(event);
    window->pollEvents();

    // SDL_PushEvent queues notifications without changing device state.
    auto* native = SDL_GetWindowFromID(window->id().get());
    const bool keyboardFocus = SDL_GetKeyboardFocus() == native;
    EXPECT_EQ(window->isFocused(), keyboardFocus);
    EXPECT_EQ(
        window->isPressed(Key::w),
        keyboardFocus && SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_W]
    );
    EXPECT_FALSE(window->isPressed(Key::w));

    Vec2f position{0.0f};
    const auto buttons = SDL_GetMouseState(&position.x, &position.y);
    const bool mouseFocus = SDL_GetMouseFocus() == native;
    EXPECT_EQ(
        window->isPressed(MouseButton::left),
        mouseFocus && (buttons & SDL_BUTTON_LMASK) != 0
    );
    EXPECT_EQ(window->mousePosition(), mouseFocus ? position : Vec2f{0.0f});
    EXPECT_FALSE(window->isPressed(MouseButton::left));
    EXPECT_FALSE(window->isPressed(Key::unknown));
    EXPECT_FALSE(window->isPressed(static_cast<Key>(65535)));
    EXPECT_FALSE(window->isPressed(MouseButton::count));
    EXPECT_FALSE(window->isPressed(static_cast<MouseButton>(255)));
}

TEST_F(WindowInput, DrainsAllRepeatNotifications) {
    int initialPresses = 0;
    int repeats = 0;
    bus.on<KeyPressedEvent>([&](const auto& event) {
        if (event.repeat) {
            ++repeats;
        } else {
            ++initialPresses;
        }
    });
    key(SDL_EVENT_KEY_DOWN, SDL_SCANCODE_A);
    constexpr int repeatCount = 40;
    for (int i = 0; i < repeatCount; ++i) {
        key(SDL_EVENT_KEY_DOWN, SDL_SCANCODE_A, true);
    }
    window->pollEvents();
    events.dispatch();
    EXPECT_EQ(initialPresses, 1);
    EXPECT_EQ(repeats, repeatCount);
    window->pollEvents();
    events.dispatch();
    EXPECT_EQ(repeats, repeatCount);
}

TEST_F(WindowInput, TranslatesMousePayloadsAndFlippedWheelDirection) {
    int moved = 0;
    int scrolled = 0;
    int pressed = 0;
    int released = 0;
    bus.on<MouseMovedEvent>([&](const auto& event) {
        ++moved;
        EXPECT_EQ(event.window, window->id());
        EXPECT_EQ(event.position, (Vec2f{12.5f, 24.5f}));
        EXPECT_EQ(event.delta, (Vec2f{1.5f, -2.0f}));
    });
    bus.on<MouseWheelEvent>([&](const auto& event) {
        ++scrolled;
        EXPECT_EQ(event.window, window->id());
        EXPECT_EQ(event.delta, (Vec2f{-0.5f, -2.0f}));
    });
    bus.on<MouseButtonPressedEvent>([&](const auto& event) {
        ++pressed;
        EXPECT_EQ(event.window, window->id());
        EXPECT_EQ(event.button, MouseButton::back);
        EXPECT_EQ(event.position, (Vec2f{10, 20}));
    });
    bus.on<MouseButtonReleasedEvent>([&](const auto& event) {
        ++released;
        EXPECT_EQ(event.window, window->id());
        EXPECT_EQ(event.button, MouseButton::back);
    });
    SDL_Event event{};
    event.type = SDL_EVENT_MOUSE_MOTION;
    event.motion.windowID = window->id().get();
    event.motion.x = 12.5f;
    event.motion.y = 24.5f;
    event.motion.xrel = 1.5f;
    event.motion.yrel = -2;
    push(event);
    event = {};
    event.type = SDL_EVENT_MOUSE_WHEEL;
    event.wheel.windowID = window->id().get();
    event.wheel.x = 0.5f;
    event.wheel.y = 2;
    event.wheel.direction = SDL_MOUSEWHEEL_FLIPPED;
    push(event);
    button(SDL_EVENT_MOUSE_BUTTON_DOWN, SDL_BUTTON_X1);
    button(SDL_EVENT_MOUSE_BUTTON_UP, SDL_BUTTON_X1);
    button(SDL_EVENT_MOUSE_BUTTON_DOWN, 255);
    window->pollEvents();
    events.dispatch();
    EXPECT_EQ(moved, 1);
    EXPECT_EQ(scrolled, 1);
    EXPECT_EQ(pressed, 1);
    EXPECT_EQ(released, 1);
}

TEST_F(WindowInput, RoutesSharedQueueToEachWindowsBus) {
    EventSystem otherEvents;
    auto otherBus = otherEvents.createBus();
    auto other =
        std::make_unique<SDLWindowBase>(Config{}, otherEvents.createBus());
    window->pollEvents();
    events.dispatch();
    otherEvents.dispatch();

    int firstKeys = 0;
    int otherKeys = 0;
    Opt<WindowResizedEvent> resized;
    Opt<WindowFocusChangedEvent> focused;
    bus.on<KeyPressedEvent>([&](const auto& event) {
        ++firstKeys;
        EXPECT_EQ(event.window, window->id());
        EXPECT_EQ(event.key, Key::b);
    });
    const auto otherId = other->id();
    otherBus.on<KeyPressedEvent>([&](const auto& event) {
        ++otherKeys;
        EXPECT_EQ(event.window, otherId);
        EXPECT_EQ(event.key, Key::a);
    });
    otherBus.on<WindowResizedEvent>([&](const auto& event) {
        resized = event;
    });
    otherBus.on<WindowFocusChangedEvent>([&](const auto& event) {
        focused = event;
    });

    key(SDL_EVENT_KEY_DOWN, SDL_SCANCODE_A, false, otherId.get());
    key(SDL_EVENT_KEY_DOWN, SDL_SCANCODE_B);
    SDL_Event event{};
    event.type = SDL_EVENT_WINDOW_RESIZED;
    event.window.windowID = otherId.get();
    event.window.data1 = 640;
    event.window.data2 = 480;
    push(event);
    event.type = SDL_EVENT_WINDOW_FOCUS_LOST;
    push(event);
    window->pollEvents();
    events.dispatch();
    EXPECT_EQ(firstKeys, 1);
    EXPECT_EQ(otherKeys, 0);
    otherEvents.dispatch();
    EXPECT_EQ(otherKeys, 1);
    ASSERT_TRUE(resized);
    EXPECT_EQ(resized->window, otherId);
    EXPECT_EQ(resized->width, 640);
    EXPECT_EQ(resized->height, 480);
    ASSERT_TRUE(focused);
    EXPECT_EQ(focused->window, otherId);
    EXPECT_FALSE(focused->focused);

    other.reset();
    EXPECT_NE(SDL_WasInit(SDL_INIT_VIDEO), 0u);
    EXPECT_NE(SDL_GetWindowFromID(window->id().get()), nullptr);
    key(SDL_EVENT_KEY_DOWN, SDL_SCANCODE_A, false, otherId.get());
    key(SDL_EVENT_KEY_DOWN, SDL_SCANCODE_B);
    window->pollEvents();
    events.dispatch();
    otherEvents.dispatch();
    EXPECT_EQ(firstKeys, 2);
    EXPECT_EQ(otherKeys, 1);
}

TEST_F(WindowInput, RoutesWindowCloseWithoutRequestingApplicationQuit) {
    EventSystem otherEvents;
    auto otherBus = otherEvents.createBus();
    SDLWindowBase other{Config{}, otherEvents.createBus()};
    window->pollEvents();
    events.dispatch();
    otherEvents.dispatch();

    int firstClosed = 0;
    int otherClosed = 0;
    int quit = 0;
    bus.on<WindowCloseRequestedEvent>([&](const auto& event) {
        ++firstClosed;
        EXPECT_EQ(event.window, window->id());
    });
    otherBus.on<WindowCloseRequestedEvent>([&](const auto& event) {
        ++otherClosed;
        EXPECT_EQ(event.window, other.id());
    });
    bus.on<QuitRequestedEvent>([&](const auto&) { ++quit; });
    otherBus.on<QuitRequestedEvent>([&](const auto&) { ++quit; });

    SDL_Event event{};
    event.type = SDL_EVENT_WINDOW_CLOSE_REQUESTED;
    event.window.windowID = other.id().get();
    push(event);
    window->pollEvents();
    events.dispatch();
    otherEvents.dispatch();
    EXPECT_EQ(firstClosed, 0);
    EXPECT_EQ(otherClosed, 1);
    EXPECT_EQ(quit, 0);
    EXPECT_NE(SDL_GetWindowFromID(other.id().get()), nullptr);

    event.window.windowID = window->id().get();
    push(event);
    other.pollEvents();
    events.dispatch();
    otherEvents.dispatch();
    EXPECT_EQ(firstClosed, 1);
    EXPECT_EQ(otherClosed, 1);
    EXPECT_EQ(quit, 0);
}

TEST_F(WindowInput, PreservesExternalSDLVideoOwnership) {
    ASSERT_TRUE(SDL_InitSubSystem(SDL_INIT_VIDEO));
    window.reset();
    EXPECT_NE(SDL_WasInit(SDL_INIT_VIDEO), 0u);
    auto* native = SDL_CreateWindow("External", 100, 100, 0);
    ASSERT_NE(native, nullptr);
    window = std::make_unique<SDLWindowBase>(Config{}, events.createBus());
    int received = 0;
    bus.on<KeyPressedEvent>([&](const auto&) { ++received; });
    key(SDL_EVENT_KEY_DOWN, SDL_SCANCODE_A, false, SDL_GetWindowID(native));
    window->pollEvents();
    events.dispatch();
    EXPECT_EQ(received, 0);
    SDL_DestroyWindow(native);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    EXPECT_NE(SDL_WasInit(SDL_INIT_VIDEO), 0u);
}

TEST_F(WindowInput, MapsDesktopKeyGroupsAndIgnoresUnsupportedScancodes) {
    std::vector<Key> received;
    bus.on<KeyPressedEvent>([&](const auto& event) {
        received.push_back(event.key);
    });
    key(SDL_EVENT_KEY_DOWN, SDL_SCANCODE_LCTRL);
    key(SDL_EVENT_KEY_DOWN, SDL_SCANCODE_RCTRL);
    key(SDL_EVENT_KEY_DOWN, SDL_SCANCODE_0);
    key(SDL_EVENT_KEY_DOWN, SDL_SCANCODE_KP_0);
    key(SDL_EVENT_KEY_DOWN, SDL_SCANCODE_F24);
    key(SDL_EVENT_KEY_DOWN, SDL_SCANCODE_UNKNOWN);
    key(SDL_EVENT_KEY_DOWN, SDL_SCANCODE_SLEEP);
    window->pollEvents();
    events.dispatch();
    const std::vector expected{
        Key::leftControl, Key::rightControl, Key::digit0, Key::numpad0, Key::f24
    };
    EXPECT_EQ(received, expected);
}

TEST_F(WindowInput, PostsApplicationQuitToPollingWindowsBus) {
    bool quit = false;
    bus.on<QuitRequestedEvent>([&](const auto&) { quit = true; });
    SDL_Event event{};
    event.type = SDL_EVENT_QUIT;
    push(event);
    window->pollEvents();
    EXPECT_FALSE(quit);
    events.dispatch();
    EXPECT_TRUE(quit);
}

}  // namespace
