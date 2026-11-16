# Cameras and view parameters

[`Camera`](../src/engine/starlight/camera/Camera.hh) exposes
`matrices(aspectRatio)`, returning an owned `CameraMatrices` value containing
`worldToView` and `viewToClip`. Its `worldToClip()` helper multiplies projection
by view. Camera objects stay on the engine side; renderer messages contain
copied shader blocks rather than camera references or handles.
Camera implementations inherit the interface's noncopyable/nonmovable contract;
matrix snapshots remain copyable values.

## Euler pose and perspective

[`EulerCamera`](../src/engine/starlight/camera/EulerCamera.cpp) stores position,
yaw, pitch, and roll in radians, plus vertical field of view and near/far distances.
Its default pose is the origin with zero rotation. Perspective defaults to a
60-degree vertical field of view, near distance 0.1, and far distance 1000.

The camera uses right-handed world/view coordinates: +X right, +Y up, looking
along -Z at zero rotation. The camera-to-world transform is
`T(position) * Ry(yaw) * Rx(pitch) * Rz(roll)`; the view matrix applies its inverse.
Pitch is not clamped and roll is independent. This retains Euler-angle
singularities; the implementation constructs orientation directly instead of
using a world-up `lookAt` calculation.
A fixed up vector would constrain roll and become parallel to the forward vector
at vertical orientations. A rotated up vector could be used with `lookAt`, but
constructing it would require the camera orientation already calculated here.

Projection uses `glm::perspectiveRH_ZO` explicitly, mapping near/far depth to
0/1. The camera has no backend selector. Setters reject nonfinite values,
unrepresentable matrices, and invalid projection ranges (`0 < fov < pi`,
`0 < near < far`) without changing the previous state. `matrices()` rejects
nonfinite/nonpositive aspect ratios and nonfinite projection results. Matrices
are calculated on demand without mutable caches.
Pose and projection predicates are local camera helpers; matrix finiteness uses
the shared [`isFinite(Mat4f)`](../src/engine/starlight/math/Math.hh) utility.

The caller supplies the aspect ratio for each view. The
[sandbox](../src/sandbox/main.cpp) uses
[`Window::drawableSize()`](../src/engine/starlight/window/Window.hh), implemented
with SDL's pixel-size query, and skips zero-sized views. Camera code does not
subscribe to window events.

## Input controllers

[`FlyCameraController`](../src/engine/starlight/camera/FlyCameraController.cpp)
updates an `EulerCamera` from the `Window` interface and elapsed seconds:

- WASD moves along camera-local forward/right axes; Q/E moves along world Y.
- Combined movement is limited to 3 world units per second.
- Holding the right mouse button changes yaw/pitch, at 0.003 radians per pixel.
- Z/C changes roll, at 1.5 radians per second.

Mouse rotation uses pixel displacement independently of elapsed time. The
controller resets its previous cursor position when dragging stops, keyboard
focus is lost, or the source window changes. The first sample of a drag only
establishes its origin. Unfocused windows do not move the camera. Invalid update
durations return an invalid-argument error. Input policy is separate from camera
matrix calculation and from the renderer. Cursor capture and warping are not
implemented, so dragging remains limited by normal cursor movement.

[`OrbitCameraController`](../src/engine/starlight/camera/OrbitCameraController.cpp)
rotates an Euler camera around the center supplied to its constructor (the
origin by default). It derives spherical coordinates from the camera's current
position, so its starting distance and angle come from that pose. The camera
must start away from the center. Each focused update faces that center and sets
roll to zero, leaving projection settings unchanged.

- A/D orbits left/right and W/S orbits up/down at 1.5 radians per second per axis.
- Q moves out and E moves in at 3 world units per second.
- Distance is clamped to 0.25–100 units and elevation to ±89 degrees.

The orbit controller validates elapsed time and center coordinates, ignores
unfocused input, and reports invalid/zero distance or unrepresentable poses through
`Result`. It stores the center, without owning a camera or window. The sandbox
uses this controller by default around the scene origin. Wheel scaling and the
left-click object-rotation toggle remain separate
sandbox controls.

## Submission and shader layout

Each [`RenderView`](../src/engine/starlight/renderer/frontend/RenderRequest.hh)
owns a target and named parameter blocks. The sandbox copies `CameraMatrices`
into a `view` block for that view and supplies a model-to-world `transform` block
per draw. Its shader computes `projection * view * model * position` using
column-major matrices and `mul(matrix, vector)`.

The [shader](../assets/shaders/simple.slang) declares a 128-byte view block
(two float4x4 matrices) and a 64-byte transform block. Shader reflection determines
their bindings. Neither the renderer nor the backend reserves a camera block
name or depends on an Euler camera type.

[`RenderRequestBuilder`](../src/engine/starlight/renderer/frontend/RenderRequestBuilder.cpp)
validates the complete material/draw/view block union for every draw/view pair.
Unknown names, missing blocks, size mismatches and duplicate names across scopes
are errors; view values do not override material or draw values. With no views,
building retains the existing material/draw completeness check. Raw items seeded
into a builder continue to rely on backend validation.

The builder copies view blocks when added and borrows materials until build.
Later camera edits do not change collected view bytes. Requests retain view and
draw blocks separately; [`Renderer::recordFrame()`](../src/engine/starlight/renderer/frontend/Renderer.cpp)
combines the current view's blocks with each item's blocks before the existing
RHI binding call. Submission owns a copy, so subsequent camera/request changes do
not mutate accepted frames. Initial uploads still occur per draw.
Resources selected through view bytes must be declared in the request's resource
list or the material/draw dependency declarations; byte blocks do not infer handles.

Every view renders the same scene items. All shaders used in a view must accept
its supplied blocks. Separate per-view visibility lists, UI passes and depth
attachments/testing are not implemented by this camera path.

## Checks

[`Camera.test.cpp`](../src/engine/tests/camera/Camera.test.cpp) checks axes,
rotation order, perspective depth/aspect, rejected inputs, independent snapshots,
and controller movement/drag/focus behavior, plus orbit targeting, radius and
elevation limits, and rejected orbit inputs. Renderer
[builder tests](../src/engine/tests/renderer/Materials.test.cpp) check per-view
validation and snapshots; [recording tests](../src/engine/tests/renderer/Textures.test.cpp)
check independent view blocks after queueing. The
[Metal pixel test](../src/engine/tests/renderer/MetalTextures.test.cpp) binds both
view matrices and a separate object matrix and checks transformed coverage.
