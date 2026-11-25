# BlueJay Scripting API

Reference pages for every class and struct in `Source/`. The folders here mirror `Source/`, so the page for a type sits in the same place as its header: `Source/Runtime/Core/ArenaAllocator.h` is documented in `Documentation/Runtime/Core/Arena.md`.

Pages follow the layout of Unity's Scripting API reference. Each page has a header line (kind, namespace, base class, source file), a **Description** with an example, summary tables of members, and a section per member with its **Declaration**, **Parameters**, **Returns** and **Description**.

Pages for code that is not compiled yet (Physics and Editor) carry a warning at the top. Folders with no classes yet hold a `.gitkeep`, as in `Source/`. Vendored libraries in `Source/ThirdParty` are documented by their own projects; see `Source/ThirdParty/VERSIONS.md`.

For the engine's plans, see [Game Engine Roadmap](Game%20Engine%20Roadmap.md).

## Runtime

### Application

| Type | Description |
| --- | --- |
| [AppConfig](Runtime/Application/AppConfig.md) | Settings for the application that `Run` creates. |
| [Game](Runtime/Application/Game.md) | Base class for a game that the main loop drives. Also documents `Run`. |

### Core

| Type | Description |
| --- | --- |
| [Arena](Runtime/Core/Arena.md) | Linear allocator that hands out memory from one fixed block. |
| [PoolAllocator](Runtime/Core/PoolAllocator.md) | Fixed-capacity allocator for objects of one type. |
| [Clock](Runtime/Core/Clock.md) | Measures the time between frames. |
| [Vector2](Runtime/Core/Vector2.md) | 2D vector or point. |
| [Vector3](Runtime/Core/Vector3.md) | 3D vector, point or colour. Also documents `DotProduct`, `CrossProduct`, `Normalize`. |
| [Matrix4x4](Runtime/Core/Matrix4x4.md) | 4x4 transformation matrix. Also documents `Translation`, `RotationX`, `RotationY`, `Perspective`, `TransformPoint`. |
| [Triangle](Runtime/Core/Triangle.md) | One flat-coloured face of a mesh. |
| [ScreenVertex](Runtime/Core/ScreenVertex.md) | A vertex projected to pixel coordinates. |
| [Mesh](Runtime/Core/Mesh.md) | Triangle geometry. Also documents `MakeCube` and `MakePyramid`. |
| [Color4](Runtime/Core/Color4.md) | RGBA colour with 0–255 channels. |

### InputSystem

| Type | Description |
| --- | --- |
| [InputSystem](Runtime/InputSystem/InputSystem.md) | Action-based keyboard, mouse and gamepad input on top of SDL3. |
| [InputAction](Runtime/InputSystem/InputAction.md) | A named action and its bindings. Also documents `ActionType` and the binding constants. |
| [BindingHotkeys](Runtime/InputSystem/BindingHotkeys.md) | Rebind actions with F1, F2, F3 and so on. |

### Physics

| Type | Description |
| --- | --- |
| [Transform3D](Runtime/Physics/Transform3D.md) | 3D position, rotation and scale with edge helpers. |
| [RigidBody3D](Runtime/Physics/RigidBody3D.md) | Oriented-box rigid body. |
| [Transform2D](Runtime/Physics/Transform2D.md) | 2D position and size with overlap tests. |
| [RigidBody2D](Runtime/Physics/RigidBody2D.md) | Simple 2D body in screen coordinates. |
| [PhysicsBody](Runtime/Physics/PhysicsBody.md) | A body as the physics world sees it: an id and a rigid body. |
| [ContactEvent](Runtime/Physics/ContactEvent.md) | Two bodies started or stopped touching. Also documents `ContactPhase`. |
| [PhysicsWorld](Runtime/Physics/PhysicsWorld.md) | Resolves collisions over every body and reports contacts. |

### Rendering

| Type | Description |
| --- | --- |
| [Camera](Runtime/Rendering/Camera.md) | First-person camera with yaw and pitch. |
| [CameraController](Runtime/Rendering/CameraController.md) | Flies a camera from player input. |
| [Renderer](Runtime/Rendering/Renderer.md) | Software rasteriser for flat-shaded meshes. |
| [Window](Runtime/Rendering/Window.md) | SDL3 window that presents a CPU framebuffer. |
| [ClipVertex](Runtime/Rendering/ClipVertex.md) | *Internal.* Clip-space vertex used inside the renderer. |

### Not documented yet

`Audio`, `Lighting`, `Networking`, `ParticleSystem`, `SaveSystem`, `SceneManagement`, `UI` and `VR` have no active source files. Their code, where there is any, is parked in `.backup` files, which are not documented here.

## Editor

| Type | Description |
| --- | --- |
| [WorldEditor](Editor/WorldEditor.md) | The in-game world editor. |
| [EditorCamera](Editor/EditorCamera.md) | The editor's fly camera. |
| [EditorGizmo](Editor/EditorGizmo.md) | Translate, rotate and scale manipulator. Also documents `GizmoMode` and `GizmoHandle`. |
| [GizmoTransform](Editor/GizmoTransform.md) | Transform value the gizmo reads and writes. |
| [EditorEdit](Editor/EditorEdit.md) | One step in the undo history. |
| [PickResult](Editor/PickResult.md) | Result of a mouse ray into the scene. Also documents `PickFilter` and the ray and snapping helpers. |

## Tests

| Type | Description |
| --- | --- |
| [Vec3](Tests/core/Vec3.md) | *Test fixture.* 12-byte struct for the pool slot-size regression test. |
| [OverAligned](Tests/core/OverAligned.md) | *Test fixture.* 32-byte-aligned struct for the pool alignment test. |
| [Counted](Tests/core/Counted.md) | *Test fixture.* Counts live instances to test constructor and destructor calls. |
