# EditorEdit

struct in `Editor` / Implemented in: [`Editor/WorldEditor.h`](../../Source/Editor/WorldEditor.h)

> [!WARNING]
> **Not built yet.** `Source/Editor` is not part of any CMake target. It uses raylib, which is not in this repository, and game types such as `GameObject` and `SceneManagement::Scene`, which exist only in `.backup` files or not at all.

## Description

One step in the World Editor's undo history.

The [WorldEditor](WorldEditor.md) records an `EditorEdit` for each change it can undo, and Ctrl+Z reverts the most recent one. There are two kinds:

| Kind | Stores | Undo does |
| --- | --- | --- |
| `EDIT_TRANSFORM` | The object's id and its position, rotation and scale from **before** the change. | Restores those values. |
| `EDIT_SPAWN` | The new object's id only. | Deletes the object. |

Deleting an object cannot be undone. Restoring a deleted object from a copy would lose its real type (a copy of a derived game object made through its base class keeps only the base part), and the source prefers no undo to an undo that silently restores the wrong thing.

The history keeps the last 128 edits.

```cpp
// Recorded by the World Editor when a gizmo drag starts...
Editor::EditorEdit edit{ Editor::EditorEdit::EDIT_TRANSFORM, object->id,
                         start.position, start.rotation, start.scale };

// ...and when an object is placed
Editor::EditorEdit spawn{ Editor::EditorEdit::EDIT_SPAWN, newObject->id };
```

Additional resources: [WorldEditor](WorldEditor.md), [GizmoTransform](GizmoTransform.md).

## Properties

| Property | Description |
| --- | --- |
| [kind](#editoreditkind) | Which kind of edit this is. |
| [id](#editoreditid) | Id of the object the edit applies to. |
| [position](#editoreditposition) | Position before the change. Transform edits only. |
| [rotation](#editoreditrotation) | Rotation before the change. Transform edits only. |
| [scale](#editoreditscale) | Scale before the change. Transform edits only. |

## Related Types

| Type | Description |
| --- | --- |
| [Kind](#editoreditkind-enum) | The kinds of edit. |

---

## EditorEdit.kind

### Declaration

```cpp
Kind kind = EDIT_TRANSFORM;
```

### Description

Which kind of edit this is. See [Kind](#editoreditkind-enum).

---

## EditorEdit.id

### Declaration

```cpp
std::uint64_t id = 0;
```

### Description

Id of the object the edit applies to. Ids are stored instead of pointers because they stay valid when objects are added or removed.

---

## EditorEdit.position

### Declaration

```cpp
Vector3 position = {};
```

### Description

Position before the change. Only used by `EDIT_TRANSFORM`.

---

## EditorEdit.rotation

### Declaration

```cpp
Quaternion rotation = QuaternionIdentity();
```

### Description

Rotation before the change. Only used by `EDIT_TRANSFORM`.

---

## EditorEdit.scale

### Declaration

```cpp
Vector3 scale = Vector3One();
```

### Description

Scale before the change. Only used by `EDIT_TRANSFORM`.

---

## EditorEdit.Kind (enum)

### Declaration

```cpp
enum Kind { EDIT_TRANSFORM, EDIT_SPAWN };
```

### Description

| Value | Description |
| --- | --- |
| `EDIT_TRANSFORM` | An object was moved, rotated or scaled. |
| `EDIT_SPAWN` | An object was created. |
