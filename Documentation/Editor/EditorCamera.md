# EditorCamera

struct in `Editor` / Inherits from: raylib `Camera3D` / Implemented in: [`Editor/WorldEditor.h`](../../Source/Editor/WorldEditor.h), [`Editor/EditorCamera.cpp`](../../Source/Editor/EditorCamera.cpp)

> [!WARNING]
> **Not built yet.** `Source/Editor` is not part of any CMake target. It uses raylib, which is not in this repository, and game types such as `GameObject` and `SceneManagement::Scene`, which exist only in `.backup` files or not at all.

## Description

The World Editor's fly camera.

While the editor is open, `EditorCamera` takes over from the game camera. Hold the right mouse button and move the mouse to look around, and fly with the movement actions:

| Input | Effect |
| --- | --- |
| Right mouse button + mouse | Look around. |
| Move Forward / Backward | Fly along the look direction, including up and down. |
| Move Left / Right | Strafe horizontally. |
| Editor_Up / Editor_Down | Rise and sink along world Y. |
| Sprint (held) | Double speed. |

Speed is 30 units per second (60 while sprinting) at any frame rate. A single long frame, such as during an asset load, moves the camera at most three frames' worth.

The camera stays out of the way of ImGui: looking is ignored while the mouse is over an editor window, and movement is ignored while a text field has the keyboard, so typing a name with W, A, S or D in it does not fly the camera away.

`EditorCamera` keeps its own pose in its inherited `Camera3D` fields and writes it into the camera passed to [Update](#editorcameraupdate) each frame.

```cpp
Editor::EditorCamera& editorCam = Editor::WorldEditor::getInstance().editorCamera;

// When the editor opens: start from the game camera's pose
editorCam.SyncFrom(sceneCamera);

// Each frame while the editor is open
editorCam.Update(&sceneCamera);

// Frame the selected object
editorCam.FocusOn(selected.translation, 6.0f);
```

Additional resources: [WorldEditor](WorldEditor.md), [Camera](../Runtime/Rendering/Camera.md), [InputSystem](../Runtime/InputSystem/InputSystem.md).

## Properties

| Property | Description |
| --- | --- |
| [yaw](#editorcamerayaw) | Horizontal look angle, in radians. |
| [pitch](#editorcamerapitch) | Vertical look angle, in radians. |
| [isSeeded](#editorcameraisseeded) | Whether the camera has adopted a starting pose. |
| [Direction vectors](#editorcamera-direction-vectors) | Basis vectors rebuilt every update. |
| [sensitivity](#editorcamerasensitivity-and-lookrotation) | Not used. |
| [lookRotation](#editorcamerasensitivity-and-lookrotation) | Not used. |

## Public Methods

| Method | Description |
| --- | --- |
| [Update](#editorcameraupdate) | Applies one frame of look and movement and writes the pose into a camera. |
| [SyncFrom](#editorcamerasyncfrom) | Adopts another camera's position and look direction. |
| [FocusOn](#editorcamerafocuson) | Moves back from a point along the current look direction. |

## Inherited Members

### Properties (raylib `Camera3D`)

| Property | Description |
| --- | --- |
| `position` | The editor camera's position. |
| `target`, `up`, `fovy`, `projection` | Inherited but not used by `EditorCamera`; it writes `target` and `up` into the camera passed to [Update](#editorcameraupdate) instead. |

---

## EditorCamera.yaw

### Declaration

```cpp
float yaw = 0.0f;
```

### Description

Horizontal look angle, in radians. 0 looks down +Z. Stored on the camera, rather than in a function, so it can be recovered from the camera the editor takes over from.

---

## EditorCamera.pitch

### Declaration

```cpp
float pitch = 0.0f;
```

### Description

Vertical look angle, in radians. Positive looks up. Kept within ±1.5 radians: at exactly straight up or down the strafe directions collapse to zero and the view would roll unpredictably.

---

## EditorCamera.isSeeded

### Declaration

```cpp
bool isSeeded = false;
```

### Description

Whether the camera has adopted a starting pose with [SyncFrom](#editorcamerasyncfrom). If it has not when [Update](#editorcameraupdate) first runs, `Update` calls `SyncFrom` itself, so the view never jumps to the origin.

---

## EditorCamera direction vectors

### Declaration

```cpp
Vector3 forward = {};
Vector3 back = {};
Vector3 right = {};
Vector3 left = {};
Vector3 up = {};
Vector3 down = {};
```

### Description

The camera's basis, rebuilt from [yaw](#editorcamerayaw) and [pitch](#editorcamerapitch) on every [Update](#editorcameraupdate).

| Vector | Value |
| --- | --- |
| `forward` | Unit look direction, including pitch. |
| `back` | `-forward`. |
| `right` | `(-forward.z, 0, forward.x)`: horizontal. Looking down +Z, right is -X. |
| `left` | `-right`. |
| `up`, `down` | World +Y and -Y. |

`right` and `left` are not normalised, so their length is `cos(pitch)`. As a result, strafing slows down as the camera looks further up or down.

---

## EditorCamera.sensitivity and lookRotation

### Declaration

```cpp
Vector2 sensitivity = Vector2{ 0.01f, 0.01f };
Vector2 lookRotation = {};
```

### Description

Declared but not read anywhere in the editor. Look speed is fixed at 0.005 radians per mouse pixel. Changing these fields has no effect.

---

## EditorCamera.Update

### Declaration

```cpp
void Update(Camera3D* camera);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `camera` | The camera to drive, normally the scene's main camera. `nullptr` does nothing. |

### Description

Applies one frame of look and movement and writes the pose into `camera`.

1. If the camera has not been seeded, adopts `*camera`'s pose with [SyncFrom](#editorcamerasyncfrom).
2. If the right mouse button is held and ImGui does not want the mouse, turns by the mouse movement.
3. If ImGui does not want the keyboard, moves by the movement actions.
4. Always rebuilds the [direction vectors](#editorcamera-direction-vectors) and writes `position`, `target` (10 units ahead) and `up` into `camera`, so the target follows the position even on frames with no input.

Selecting objects with the mouse is not handled here; the [WorldEditor](WorldEditor.md) does it so it can be ordered after the gizmo and placement tools.

---

## EditorCamera.SyncFrom

### Declaration

```cpp
void SyncFrom(const Camera3D& camera);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `camera` | The camera whose pose to adopt. |

### Description

Adopts another camera's position and look direction, and marks the editor camera as seeded.

Yaw and pitch are worked out from `camera.target - camera.position`, so the view continues exactly where the game camera was pointing. If the target and position are the same point, the current angles are kept. The [WorldEditor](WorldEditor.md) calls this every time the editor is opened with F1.

---

## EditorCamera.FocusOn

### Declaration

```cpp
void FocusOn(Vector3 target, float distance);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `target` | The point to frame. |
| `distance` | How far back from `target` to put the camera. Values below 1 are treated as 1. |

### Description

Moves the camera back from `target` along the current look direction, so the point ends up straight ahead at `distance`. The look direction is not changed, which keeps the user oriented. If the camera has no look direction yet, it uses +Z.
