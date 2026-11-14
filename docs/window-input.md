# Windows, keyboard, and mouse

[`Window`](../src/engine/starlight/window/Window.hh) provides event polling,
held-key/button queries, keyboard focus, and mouse position.
[`WindowTypes.hh`](../src/engine/starlight/window/WindowTypes.hh) defines physical
keys, mouse buttons, modifiers, and window IDs without SDL dependencies.

[`SDLWindowBase`](../src/engine/starlight/platform/sdl/SDLWindow.cpp) reads SDL's
native input state directly. Native handlers translate events into typed
notifications through the window's injected `EventBus`. Window/input payloads
live in [`window/WindowEvents.hh`](../src/engine/starlight/window/WindowEvents.hh).
The application-wide `QuitRequestedEvent` remains in
[`event/Events.hh`](../src/engine/starlight/event/Events.hh).
Generic event transport has no dependency on window types.

`SDLMetalWindow` additionally implements `MetalRenderSurfaceProvider`; renderer
access remains separate from the window interface. The Vulkan surface provider
is a stub and does not establish Vulkan rendering support.

## Polling and multiple windows

Call `pollEvents()` once per engine-loop iteration through one live SDL window.
It drains SDL's shared queue. Each native window stores a non-owning pointer to
its `SDLWindowBase` in an SDL window property. Events are routed through that
property to the owning window's bus, even when another window performs polling.
Events for destroyed windows or native windows without a Starlight owner are
ignored. Application quit requests go to the polling window's bus.
Window close requests produce `WindowCloseRequestedEvent` on the owning window's
bus. Callers decide whether to close that window; the adapter leaves it alive
and forwards any separate application quit event from SDL.
Record close intent in the callback and destroy windows after dispatch returns.
The [broker](../src/engine/starlight/event/details/EventBroker.cpp) holds its
handler mutex during callbacks; window destruction unregisters its
[`EventBus`](../src/engine/starlight/event/EventBus.cpp) through that same mutex.

Each window acquires and releases an SDL video subsystem reference. Destroying
one window preserves SDL while other windows or external subsystem references
remain. Full SDL cleanup runs when no initialized subsystems remain.

All window queries, polling, creation, and destruction use the main thread,
checked with `log::expect` in Debug and Release.

The [sandbox](../src/sandbox/main.cpp) polls before dispatching notifications:

```cpp
window->pollEvents();
events.dispatch();

if (window->isPressed(Key::w)) {
    // Continuous movement uses SDL's current held state.
}
```

## Queries and notifications

- `isPressed(Key)` queries `SDL_GetKeyboardState` and returns false unless this
  window has keyboard focus. Keys identify physical positions rather than typed
  characters.
- `isPressed(MouseButton)` queries `SDL_GetMouseState` and returns false unless
  this window has mouse focus. Keyboard focus and mouse focus can differ.
- `isFocused()` reports native keyboard focus.
- `mousePosition()` reports SDL's window-relative coordinates for the
  mouse-focused window, or zero when this window lacks mouse focus.
- Unknown or out-of-range key/button values read as false. The adapter maps
  desktop letters, digits, punctuation, navigation, F1–F24, keypad, modifier
  keys, and five mouse buttons.

SDL updates its state during event pumping; queries work before bus dispatch.
A complete press/release between polls can leave a key unpressed in the native
snapshot. Use separate pressed/released notifications for those transitions,
including short taps. Key-press events preserve SDL's repeat flag and event-time
modifiers. Mouse events carry event-time positions and movement deltas; wheel
events normalize SDL's flipped direction. Focus and resize notifications carry
the source window ID.

The [event broker](../src/engine/starlight/event/details/EventBroker.cpp) groups
notifications by type and does not preserve order across types. Queries inside a
callback observe current SDL state, while payloads describe the captured event.
Events posted during dispatch wait for the next dispatch.

The interface covers keyboard/mouse I/O without text input, action mapping,
or cursor-capture controls. The sandbox uses held-state WASD movement and typed
events for wheel scaling, a left-click rotation toggle, and Escape to quit.

## Checks

[`Window.test.cpp`](../src/engine/tests/window/Window.test.cpp) uses SDL's dummy
video driver to check native-query agreement, event translation, short taps,
repeat delivery, per-window bus and close-request routing, destroyed/foreign-window
handling, and subsystem lifetime. `SDL_PushEvent` injects notifications without changing
SDL's physical device state; these tests do not simulate hardware-held inputs.
