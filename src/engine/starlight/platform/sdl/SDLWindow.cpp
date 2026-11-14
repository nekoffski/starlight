#include "SDLWindow.hh"

#include <algorithm>
#include <array>

#include "starlight/core/Core.hh"
#include "starlight/event/Events.hh"
#include "starlight/window/WindowEvents.hh"

namespace sl {
namespace {

struct KeyMapping {
    SDL_Scancode scancode;
    Key key;
};

constexpr auto keyMappings = std::to_array<KeyMapping>({
    {SDL_SCANCODE_A, Key::a},
    {SDL_SCANCODE_B, Key::b},
    {SDL_SCANCODE_C, Key::c},
    {SDL_SCANCODE_D, Key::d},
    {SDL_SCANCODE_E, Key::e},
    {SDL_SCANCODE_F, Key::f},
    {SDL_SCANCODE_G, Key::g},
    {SDL_SCANCODE_H, Key::h},
    {SDL_SCANCODE_I, Key::i},
    {SDL_SCANCODE_J, Key::j},
    {SDL_SCANCODE_K, Key::k},
    {SDL_SCANCODE_L, Key::l},
    {SDL_SCANCODE_M, Key::m},
    {SDL_SCANCODE_N, Key::n},
    {SDL_SCANCODE_O, Key::o},
    {SDL_SCANCODE_P, Key::p},
    {SDL_SCANCODE_Q, Key::q},
    {SDL_SCANCODE_R, Key::r},
    {SDL_SCANCODE_S, Key::s},
    {SDL_SCANCODE_T, Key::t},
    {SDL_SCANCODE_U, Key::u},
    {SDL_SCANCODE_V, Key::v},
    {SDL_SCANCODE_W, Key::w},
    {SDL_SCANCODE_X, Key::x},
    {SDL_SCANCODE_Y, Key::y},
    {SDL_SCANCODE_Z, Key::z},
    {SDL_SCANCODE_0, Key::digit0},
    {SDL_SCANCODE_1, Key::digit1},
    {SDL_SCANCODE_2, Key::digit2},
    {SDL_SCANCODE_3, Key::digit3},
    {SDL_SCANCODE_4, Key::digit4},
    {SDL_SCANCODE_5, Key::digit5},
    {SDL_SCANCODE_6, Key::digit6},
    {SDL_SCANCODE_7, Key::digit7},
    {SDL_SCANCODE_8, Key::digit8},
    {SDL_SCANCODE_9, Key::digit9},
    {SDL_SCANCODE_ESCAPE, Key::escape},
    {SDL_SCANCODE_RETURN, Key::enter},
    {SDL_SCANCODE_TAB, Key::tab},
    {SDL_SCANCODE_BACKSPACE, Key::backspace},
    {SDL_SCANCODE_SPACE, Key::space},
    {SDL_SCANCODE_MINUS, Key::minus},
    {SDL_SCANCODE_EQUALS, Key::equal},
    {SDL_SCANCODE_LEFTBRACKET, Key::leftBracket},
    {SDL_SCANCODE_RIGHTBRACKET, Key::rightBracket},
    {SDL_SCANCODE_BACKSLASH, Key::backslash},
    {SDL_SCANCODE_NONUSHASH, Key::nonUsHash},
    {SDL_SCANCODE_SEMICOLON, Key::semicolon},
    {SDL_SCANCODE_APOSTROPHE, Key::apostrophe},
    {SDL_SCANCODE_GRAVE, Key::grave},
    {SDL_SCANCODE_COMMA, Key::comma},
    {SDL_SCANCODE_PERIOD, Key::period},
    {SDL_SCANCODE_SLASH, Key::slash},
    {SDL_SCANCODE_CAPSLOCK, Key::capsLock},
    {SDL_SCANCODE_F1, Key::f1},
    {SDL_SCANCODE_F2, Key::f2},
    {SDL_SCANCODE_F3, Key::f3},
    {SDL_SCANCODE_F4, Key::f4},
    {SDL_SCANCODE_F5, Key::f5},
    {SDL_SCANCODE_F6, Key::f6},
    {SDL_SCANCODE_F7, Key::f7},
    {SDL_SCANCODE_F8, Key::f8},
    {SDL_SCANCODE_F9, Key::f9},
    {SDL_SCANCODE_F10, Key::f10},
    {SDL_SCANCODE_F11, Key::f11},
    {SDL_SCANCODE_F12, Key::f12},
    {SDL_SCANCODE_F13, Key::f13},
    {SDL_SCANCODE_F14, Key::f14},
    {SDL_SCANCODE_F15, Key::f15},
    {SDL_SCANCODE_F16, Key::f16},
    {SDL_SCANCODE_F17, Key::f17},
    {SDL_SCANCODE_F18, Key::f18},
    {SDL_SCANCODE_F19, Key::f19},
    {SDL_SCANCODE_F20, Key::f20},
    {SDL_SCANCODE_F21, Key::f21},
    {SDL_SCANCODE_F22, Key::f22},
    {SDL_SCANCODE_F23, Key::f23},
    {SDL_SCANCODE_F24, Key::f24},
    {SDL_SCANCODE_PRINTSCREEN, Key::printScreen},
    {SDL_SCANCODE_SCROLLLOCK, Key::scrollLock},
    {SDL_SCANCODE_PAUSE, Key::pause},
    {SDL_SCANCODE_INSERT, Key::insert},
    {SDL_SCANCODE_DELETE, Key::deleteKey},
    {SDL_SCANCODE_HOME, Key::home},
    {SDL_SCANCODE_END, Key::end},
    {SDL_SCANCODE_PAGEUP, Key::pageUp},
    {SDL_SCANCODE_PAGEDOWN, Key::pageDown},
    {SDL_SCANCODE_LEFT, Key::left},
    {SDL_SCANCODE_RIGHT, Key::right},
    {SDL_SCANCODE_UP, Key::up},
    {SDL_SCANCODE_DOWN, Key::down},
    {SDL_SCANCODE_NUMLOCKCLEAR, Key::numLock},
    {SDL_SCANCODE_KP_DIVIDE, Key::numpadDivide},
    {SDL_SCANCODE_KP_MULTIPLY, Key::numpadMultiply},
    {SDL_SCANCODE_KP_MINUS, Key::numpadMinus},
    {SDL_SCANCODE_KP_PLUS, Key::numpadPlus},
    {SDL_SCANCODE_KP_ENTER, Key::numpadEnter},
    {SDL_SCANCODE_KP_0, Key::numpad0},
    {SDL_SCANCODE_KP_1, Key::numpad1},
    {SDL_SCANCODE_KP_2, Key::numpad2},
    {SDL_SCANCODE_KP_3, Key::numpad3},
    {SDL_SCANCODE_KP_4, Key::numpad4},
    {SDL_SCANCODE_KP_5, Key::numpad5},
    {SDL_SCANCODE_KP_6, Key::numpad6},
    {SDL_SCANCODE_KP_7, Key::numpad7},
    {SDL_SCANCODE_KP_8, Key::numpad8},
    {SDL_SCANCODE_KP_9, Key::numpad9},
    {SDL_SCANCODE_KP_PERIOD, Key::numpadPeriod},
    {SDL_SCANCODE_KP_EQUALS, Key::numpadEqual},
    {SDL_SCANCODE_NONUSBACKSLASH, Key::nonUsBackslash},
    {SDL_SCANCODE_APPLICATION, Key::menu},
    {SDL_SCANCODE_LCTRL, Key::leftControl},
    {SDL_SCANCODE_LSHIFT, Key::leftShift},
    {SDL_SCANCODE_LALT, Key::leftAlt},
    {SDL_SCANCODE_LGUI, Key::leftMeta},
    {SDL_SCANCODE_RCTRL, Key::rightControl},
    {SDL_SCANCODE_RSHIFT, Key::rightShift},
    {SDL_SCANCODE_RALT, Key::rightAlt},
    {SDL_SCANCODE_RGUI, Key::rightMeta},
});

Key toKey(SDL_Scancode scancode) {
    const auto found =
        std::ranges::find(keyMappings, scancode, &KeyMapping::scancode);
    return found == keyMappings.end() ? Key::unknown : found->key;
}

SDL_Scancode toScancode(Key key) {
    const auto found = std::ranges::find(keyMappings, key, &KeyMapping::key);
    return found == keyMappings.end() ? SDL_SCANCODE_UNKNOWN : found->scancode;
}

KeyModifiers toModifiers(SDL_Keymod modifiers) {
    return {
        .shift = (modifiers & SDL_KMOD_SHIFT) != 0,
        .control = (modifiers & SDL_KMOD_CTRL) != 0,
        .alt = (modifiers & SDL_KMOD_ALT) != 0,
        .meta = (modifiers & SDL_KMOD_GUI) != 0,
    };
}

struct MouseButtonMapping {
    Uint8 native;
    MouseButton button;
};

constexpr auto mouseButtonMappings = std::to_array<MouseButtonMapping>({
    {SDL_BUTTON_LEFT, MouseButton::left},
    {SDL_BUTTON_MIDDLE, MouseButton::middle},
    {SDL_BUTTON_RIGHT, MouseButton::right},
    {SDL_BUTTON_X1, MouseButton::back},
    {SDL_BUTTON_X2, MouseButton::forward},
});

MouseButton toMouseButton(Uint8 button) {
    const auto found = std::ranges::find(
        mouseButtonMappings, button, &MouseButtonMapping::native
    );
    return found == mouseButtonMappings.end() ? MouseButton::unknown
                                              : found->button;
}

SDL_MouseButtonFlags toMouseButtonMask(MouseButton button) {
    const auto found = std::ranges::find(
        mouseButtonMappings, button, &MouseButtonMapping::button
    );
    return found == mouseButtonMappings.end() ? 0
                                              : SDL_BUTTON_MASK(found->native);
}

constexpr auto windowOwnerProperty = "starlight.window.owner";

}  // namespace

SDLWindowBase::SDLWindowBase(const Config& config, EventBus bus)
    : m_bus(std::move(bus)) {
    checkThread();
    log::expect(
        SDL_InitSubSystem(SDL_INIT_VIDEO), "Could not initialize SDL: {}",
        SDL_GetError()
    );
    m_window = SDL_CreateWindow("Starlight", 800, 600, SDL_WINDOW_RESIZABLE);
    log::expect(m_window, "Could not create window: {}", SDL_GetError());
    log::expect(
        SDL_SetPointerProperty(
            SDL_GetWindowProperties(m_window), windowOwnerProperty, this
        ),
        "Could not register window owner: {}", SDL_GetError()
    );
    log::trace("SDLWindow created");
}

SDLWindowBase::~SDLWindowBase() {
    checkThread();
    SDL_DestroyWindow(m_window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    if (SDL_WasInit(0) == 0) {
        SDL_Quit();
    }
    log::trace("SDLWindow destroyed");
}

void SDLWindowBase::checkThread() const {
    log::expect(SDL_IsMainThread(), "SDL windows require the main thread");
}

WindowId SDLWindowBase::id() const {
    checkThread();
    return WindowId{SDL_GetWindowID(m_window)};
}

bool SDLWindowBase::isPressed(Key key) const {
    checkThread();
    const auto scancode = toScancode(key);
    return SDL_GetKeyboardFocus() == m_window &&
           scancode != SDL_SCANCODE_UNKNOWN &&
           SDL_GetKeyboardState(nullptr)[scancode];
}

bool SDLWindowBase::isPressed(MouseButton button) const {
    checkThread();
    return SDL_GetMouseFocus() == m_window &&
           (SDL_GetMouseState(nullptr, nullptr) & toMouseButtonMask(button)) !=
               0;
}

bool SDLWindowBase::isFocused() const {
    checkThread();
    return SDL_GetKeyboardFocus() == m_window;
}

Vec2f SDLWindowBase::mousePosition() const {
    checkThread();
    Vec2f position{0.0f};
    if (SDL_GetMouseFocus() == m_window) {
        SDL_GetMouseState(&position.x, &position.y);
    }
    return position;
}

void SDLWindowBase::handleKey(const SDL_KeyboardEvent& event) {
    const auto key = toKey(event.scancode);
    if (key == Key::unknown) {
        return;
    }
    const auto modifiers = toModifiers(event.mod);
    if (event.type == SDL_EVENT_KEY_DOWN) {
        m_bus.post<KeyPressedEvent>(id(), key, modifiers, event.repeat);
    } else {
        m_bus.post<KeyReleasedEvent>(id(), key, modifiers);
    }
}

void SDLWindowBase::handleMouseButton(const SDL_MouseButtonEvent& event) {
    const auto button = toMouseButton(event.button);
    if (button == MouseButton::unknown) {
        return;
    }
    const Vec2f position{event.x, event.y};
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        m_bus.post<MouseButtonPressedEvent>(id(), button, position);
    } else {
        m_bus.post<MouseButtonReleasedEvent>(id(), button, position);
    }
}

void SDLWindowBase::handleEvent(const SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            m_bus.post<WindowCloseRequestedEvent>(id());
            break;
        case SDL_EVENT_WINDOW_RESIZED:
            m_bus.post<WindowResizedEvent>(
                static_cast<u32>(event.window.data1),
                static_cast<u32>(event.window.data2), id()
            );
            break;
        case SDL_EVENT_WINDOW_FOCUS_GAINED:
        case SDL_EVENT_WINDOW_FOCUS_LOST:
            m_bus.post<WindowFocusChangedEvent>(
                id(), event.type == SDL_EVENT_WINDOW_FOCUS_GAINED
            );
            break;
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
            handleKey(event.key);
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        case SDL_EVENT_MOUSE_BUTTON_UP:
            handleMouseButton(event.button);
            break;
        case SDL_EVENT_MOUSE_MOTION: {
            m_bus.post<MouseMovedEvent>(
                id(), Vec2f{event.motion.x, event.motion.y},
                Vec2f{event.motion.xrel, event.motion.yrel}
            );
            break;
        }
        case SDL_EVENT_MOUSE_WHEEL: {
            Vec2f delta{event.wheel.x, event.wheel.y};
            if (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) {
                delta = -delta;
            }
            m_bus.post<MouseWheelEvent>(id(), delta);
            break;
        }
    }
}

void SDLWindowBase::pollEvents() {
    checkThread();
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            m_bus.post<QuitRequestedEvent>("SDL Window quit requested");
            continue;
        }
        auto* native = SDL_GetWindowFromEvent(&event);
        if (not native) {
            continue;
        }
        auto* owner = static_cast<SDLWindowBase*>(SDL_GetPointerProperty(
            SDL_GetWindowProperties(native), windowOwnerProperty, nullptr
        ));
        if (owner) {
            owner->handleEvent(event);
        }
    }
}

#if defined(SL_USE_METAL)

SDLMetalWindow::SDLMetalWindow(const Config& config, EventBus bus)
    : SDLWindowBase(config, std::move(bus)) {
    m_metalView = SDL_Metal_CreateView(m_window);
    log::expect(
        m_metalView != nullptr, "Could not create Metal view: {}",
        SDL_GetError()
    );

    m_metalLayer =
        reinterpret_cast<CA::MetalLayer*>(SDL_Metal_GetLayer(m_metalView));
    log::expect(
        m_metalLayer != nullptr, "Could not get Metal layer: {}", SDL_GetError()
    );
}

SDLMetalWindow::~SDLMetalWindow() {
    checkThread();
    if (m_metalView) {
        SDL_Metal_DestroyView(m_metalView);
    }
}

CA::MetalLayer* SDLMetalWindow::getLayer() { return m_metalLayer; }

#elif defined(SL_USE_VULKAN)

#else
#error "Unsupported platform"
#endif

}  // namespace sl
