# GizmoTransform

struct in global namespace / Implemented in: [`Editor/EditorGizmo.h`](../../Source/Editor/EditorGizmo.h)

> [!WARNING]
> **Not built yet.** `Source/Editor` is not part of any CMake target. It uses raylib, which is not in this repository, and game types such as `GameObject` and `SceneManagement::Scene`, which exist only in `.backup` files or not at all. `Vector3` and `Quaternion` on this page are raylib's.

## Description

The position, rotation and scale that the [EditorGizmo](EditorGizmo.md) reads and writes.

`GizmoTransform` is a plain value on purpose, not a pointer to a game object. The gizmo can outlive the object it is manipulating, and a pointer would dangle once that object is destroyed. Instead, the editor looks up its selection by id once per frame, copies the transform into a `GizmoTransform`, passes it to [EditorGizmo.Update](EditorGizmo.md#editorgizmoupdate), and writes the result back. A `GizmoTransform` is only meaningful for the frame it was made in.

```cpp
GizmoTransform transform{ body.translation, body.rotation, body.scale };
gizmo.Update(camera, transform, canStartDrag);
if (gizmo.IsDragging() || gizmo.DragEnded()) {
    body.translation = transform.position;
    body.rotation    = transform.rotation;
    body.scale       = transform.scale;
    body.SyncBroadPhaseBox();
}
```

Additional resources: [EditorGizmo](EditorGizmo.md), [EditorEdit](EditorEdit.md).

## Properties

| Property | Description |
| --- | --- |
| [position](#gizmotransformposition) | World-space position, also the gizmo's pivot. |
| [rotation](#gizmotransformrotation) | Orientation. |
| [scale](#gizmotransformscale) | Size on each of the object's own axes. |

---

## GizmoTransform.position

### Declaration

```cpp
Vector3 position = {};
```

### Description

World-space position. The gizmo's handles are drawn from this point.

---

## GizmoTransform.rotation

### Declaration

```cpp
Quaternion rotation = QuaternionIdentity();
```

### Description

Orientation. The gizmo aligns its handles to it in local space and when scaling, and changes it when rotating.

---

## GizmoTransform.scale

### Declaration

```cpp
Vector3 scale = Vector3One();
```

### Description

Size on each of the object's own axes. The gizmo never makes a component smaller than [EditorGizmo.kMinimumScale](EditorGizmo.md#editorgizmokminimumscale).
