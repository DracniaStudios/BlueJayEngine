# EditorGizmo

class in global namespace / Implemented in: [`Editor/EditorGizmo.h`](../../Source/Editor/EditorGizmo.h)

> [!WARNING]
> **Not built yet.** `Source/Editor` is not part of any CMake target. It uses raylib, which is not in this repository, and game types such as `GameObject` and `SceneManagement::Scene`, which exist only in `.backup` files or not at all.

## Description

A translate, rotate and scale manipulator for the editor viewport.

The gizmo draws handles at an object's position and lets the user drag them. It owns nothing in the scene: each frame you hand it a [GizmoTransform](GizmoTransform.md), it changes that value in place, and it tells you whether it used the mouse. Writing the result back onto the object, and refreshing that object's collision box, is up to you.

| Mode | Handles | Dragging does |
| --- | --- | --- |
| `GIZMO_SELECT` | None | Nothing. The gizmo never takes the mouse. |
| `GIZMO_TRANSLATE` | Three arrows | Moves along the grabbed axis. |
| `GIZMO_ROTATE` | Three rings | Turns around the grabbed axis. Turns past 180° are allowed. |
| `GIZMO_SCALE` | Three bars with blocks, plus a centre block | Grows or shrinks along the grabbed axis. The centre block scales all three axes, dragged along the camera's right direction. |

Handles keep the same size on screen at any distance. Axis colours are red for X, green for Y and blue for Z, and the handle under the mouse or being dragged turns yellow. Pressing Esc during a drag puts the object back where the drag started.

```cpp
// Each frame, with a selected object:
GizmoTransform transform{ body.translation, body.rotation, body.scale };

const bool canStartDrag = !ImGui::GetIO().WantCaptureMouse
                       && !IsMouseButtonDown(MOUSE_BUTTON_RIGHT);
const bool gizmoOwnsMouse = gizmo.Update(camera, transform, canStartDrag);

if (gizmo.DragBegan()) { undoStart = gizmo.DragStartTransform(); }
if (gizmo.IsDragging() || gizmo.DragEnded()) { WriteBack(body, transform); }

if (!gizmoOwnsMouse && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    SelectUnderMouse();   // only when the gizmo did not use the click
}

// Later, inside BeginMode3D:
rlDisableDepthTest();
gizmo.Draw(camera, transform);
rlEnableDepthTest();
```

Additional resources: [GizmoTransform](GizmoTransform.md), [WorldEditor](WorldEditor.md), [PickResult](PickResult.md).

## Static Properties

| Property | Description |
| --- | --- |
| [kMinimumScale](#editorgizmokminimumscale) | Smallest scale the gizmo will set on any axis. |

## Properties

| Property | Description |
| --- | --- |
| [mode](#editorgizmomode) | Which manipulator is active. |
| [localSpace](#editorgizmolocalspace) | Whether handles follow the object's rotation. |
| [snapEnabled](#editorgizmosnapenabled) | Whether drags snap to fixed steps. |
| [translateSnap](#editorgizmotranslatesnap) | Snap step for moving, in world units. |
| [rotateSnapDegrees](#editorgizmorotatesnapdegrees) | Snap step for rotating, in degrees. |
| [scaleSnap](#editorgizmoscalesnap) | Snap step for scaling. |

## Public Methods

| Method | Description |
| --- | --- |
| [Update](#editorgizmoupdate) | Runs hover detection and any drag for one frame. |
| [Draw](#editorgizmodraw) | Draws the handles. |
| [Cancel](#editorgizmocancel) | Drops any hover or drag. |
| [IsDragging](#editorgizmoisdragging) | Whether a handle is being dragged. |
| [IsHovered](#editorgizmoishovered) | Whether the mouse is over a handle. |
| [DragBegan](#editorgizmodragbegan) | Whether a drag started this frame. |
| [DragEnded](#editorgizmodragended) | Whether a drag ended this frame. |
| [DragStartTransform](#editorgizmodragstarttransform) | The transform at the moment the drag started. |
| [HandleLength](#editorgizmohandlelength) | World-space handle length that keeps the gizmo a constant size on screen. |

## Related Types and Functions

| Name | Description |
| --- | --- |
| [GizmoMode](#gizmomode) | The four manipulator modes. |
| [GizmoHandle](#gizmohandle) | Which handle is hovered or grabbed. |
| [GizmoAxisColor](#gizmoaxiscolor) | Colour of an axis handle. |

---

## EditorGizmo.kMinimumScale

### Declaration

```cpp
static constexpr float kMinimumScale = MINIMUM_EDITOR_SCALE;   // 0.01
```

### Description

Smallest scale the gizmo will set on any axis. A zero or negative scale turns an object's collision box inside out, which makes it impossible to click or collide with. See [MINIMUM_EDITOR_SCALE](PickResult.md#minimum_editor_scale-and-sanitizescale).

---

## EditorGizmo.mode

### Declaration

```cpp
GizmoMode mode = GIZMO_TRANSLATE;
```

### Description

Which manipulator is active. See [GizmoMode](#gizmomode). The [WorldEditor](WorldEditor.md) sets it from its toolbar and from the 1–4 keys.

---

## EditorGizmo.localSpace

### Declaration

```cpp
bool localSpace = false;
```

### Description

When `true`, the translate and rotate handles follow the object's rotation. When `false` they line up with the world axes.

Scale handles always follow the object's rotation, whatever this is set to. A world-aligned scale handle on a rotated object would stretch it along an axis the user did not grab.

---

## EditorGizmo.snapEnabled

### Declaration

```cpp
bool snapEnabled = false;
```

### Description

Whether drags snap to [translateSnap](#editorgizmotranslatesnap), [rotateSnapDegrees](#editorgizmorotatesnapdegrees) or [scaleSnap](#editorgizmoscalesnap). Snapping applies to the change since the drag began, so an object that starts off the grid moves in whole steps from where it was.

---

## EditorGizmo.translateSnap

### Declaration

```cpp
float translateSnap = 0.5f;
```

### Description

Snap step for moving, in world units. 0 or less turns snapping off for moves.

---

## EditorGizmo.rotateSnapDegrees

### Declaration

```cpp
float rotateSnapDegrees = 15.0f;
```

### Description

Snap step for rotating, in degrees. 0 or less turns snapping off for rotation.

---

## EditorGizmo.scaleSnap

### Declaration

```cpp
float scaleSnap = 0.25f;
```

### Description

Snap step for scaling, added to or removed from the scale. 0 or less turns snapping off for scaling.

---

## EditorGizmo.Update

### Declaration

```cpp
bool Update(const Camera3D& camera, GizmoTransform& transform, bool canStartDrag);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `camera` | The camera driving the viewport. |
| `transform` | The object's transform. Changed in place during a drag. |
| `canStartDrag` | Whether a **new** drag may begin. Pass `false` while ImGui has the mouse or the camera is looking around. |

### Returns

**bool** `true` when the gizmo used the mouse this frame. The caller must not also treat the click as a selection.

### Description

Runs hover detection and any drag for one frame.

When no drag is running, it finds the handle under the mouse. A left click on a handle starts a drag if `canStartDrag` is `true`. Hovering a handle returns `true` even if the drag could not start, so a click aimed at the gizmo never deselects the object behind it.

Once a drag is running it ignores `canStartDrag`, so moving the cursor over a panel mid-drag does not drop the object. The drag keeps the mouse until the left button is released, which ends the drag and raises [DragEnded](#editorgizmodragended). Pressing Esc instead restores the transform the drag started from, and also raises `DragEnded` so that the caller writes the restored value back.

How a drag changes `transform`:

* **Translate.** The object moves along the grabbed axis to follow the cursor. The offset is always measured from the grab point, so it never drifts.
* **Rotate.** The object turns around the grabbed axis. A ring seen nearly edge-on cannot be grabbed, because the cursor angle is meaningless there.
* **Scale.** The scale grows or shrinks by the distance dragged, added to the grabbed component (or all three for the centre block) and clamped to at least [kMinimumScale](#editorgizmokminimumscale).

In `GIZMO_SELECT` mode it cancels any drag and returns `false`.

---

## EditorGizmo.Draw

### Declaration

```cpp
void Draw(const Camera3D& camera, const GizmoTransform& transform) const;
```

### Parameters

| Parameter | Description |
| --- | --- |
| `camera` | The camera driving the viewport. |
| `transform` | The transform whose handles to draw. |

### Description

Draws the handles for the current [mode](#editorgizmomode): arrows for translate, rings for rotate, and bars with blocks plus a centre block for scale. Draws nothing in `GIZMO_SELECT` mode.

Call it inside raylib's `BeginMode3D` with depth testing off, so the handles show through the object.

---

## EditorGizmo.Cancel

### Declaration

```cpp
void Cancel();
```

### Description

Drops any hover or drag and clears [DragBegan](#editorgizmodragbegan) and [DragEnded](#editorgizmodragended). It does not restore the transform.

Call it before destroying the object being dragged, and when switching to a tool that takes over the mouse.

---

## EditorGizmo.IsDragging

### Declaration

```cpp
bool IsDragging() const;
```

### Returns

**bool** `true` while a handle is grabbed.

### Description

Whether a handle is being dragged. Write the transform back to the object while this or [DragEnded](#editorgizmodragended) is `true`.

---

## EditorGizmo.IsHovered

### Declaration

```cpp
bool IsHovered() const;
```

### Returns

**bool** `true` while the mouse is over a handle.

### Description

Whether the mouse is over a handle.

---

## EditorGizmo.DragBegan

### Declaration

```cpp
bool DragBegan() const;
```

### Returns

**bool** `true` only on the frame a drag started.

### Description

Whether a drag started this frame. Together with [DragEnded](#editorgizmodragended) it lets you record exactly one undo step per drag.

---

## EditorGizmo.DragEnded

### Declaration

```cpp
bool DragEnded() const;
```

### Returns

**bool** `true` only on the frame a drag ended, by releasing the button or pressing Esc.

### Description

Whether a drag ended this frame. Compare the final transform with [DragStartTransform](#editorgizmodragstarttransform) to skip recording an undo step for a click that moved nothing.

---

## EditorGizmo.DragStartTransform

### Declaration

```cpp
const GizmoTransform& DragStartTransform() const;
```

### Returns

**const GizmoTransform&** The transform at the moment the current or last drag started.

### Description

The transform at the moment the drag started. Store it when [DragBegan](#editorgizmodragbegan) is `true` to undo the drag later.

---

## EditorGizmo.HandleLength

### Declaration

```cpp
float HandleLength(const Camera3D& camera, Vector3 center) const;
```

### Parameters

| Parameter | Description |
| --- | --- |
| `camera` | The camera driving the viewport. |
| `center` | The gizmo's pivot. |

### Returns

**float** 14% of the distance from the camera to `center`, but never less than 0.08.

### Description

World-space handle length that keeps the gizmo a constant size on screen. Without it the gizmo would be an unclickable dot far away and fill the screen up close. The minimum stops it collapsing to nothing when the camera sits on the pivot.

---

## GizmoMode

### Declaration

```cpp
enum GizmoMode { GIZMO_SELECT, GIZMO_TRANSLATE, GIZMO_ROTATE, GIZMO_SCALE, GIZMO_MODE_COUNT };
```

### Description

| Value | Description |
| --- | --- |
| `GIZMO_SELECT` | Click to select. No handles are drawn and nothing can be dragged. |
| `GIZMO_TRANSLATE` | Move along an axis. |
| `GIZMO_ROTATE` | Turn around an axis. |
| `GIZMO_SCALE` | Scale along one axis, or all three with the centre block. |
| `GIZMO_MODE_COUNT` | Number of modes. Not a mode. |

---

## GizmoHandle

### Declaration

```cpp
enum GizmoHandle { GIZMO_HANDLE_NONE = -1, GIZMO_HANDLE_X = 0, GIZMO_HANDLE_Y = 1,
                   GIZMO_HANDLE_Z = 2, GIZMO_HANDLE_UNIFORM = 3 };
```

### Description

Which part of the gizmo the mouse is over or has grabbed. `GIZMO_HANDLE_UNIFORM` is the centre block, which exists only in scale mode.

---

## GizmoAxisColor

### Declaration

```cpp
Color GizmoAxisColor(int axis, bool highlighted);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `axis` | 0 for X, 1 for Y, 2 for Z. Any other value means the centre block. |
| `highlighted` | Whether the handle is hovered or grabbed. |

### Returns

**Color** Yellow when `highlighted`. Otherwise red for X, green for Y, blue for Z, or white for any other `axis`.

### Description

Colour of an axis handle, accounting for highlighting.
