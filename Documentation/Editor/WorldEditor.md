# WorldEditor

class in `Editor` / Implemented in: [`Editor/WorldEditor.h`](../../Source/Editor/WorldEditor.h), [`Editor/WorldEditor.cpp`](../../Source/Editor/WorldEditor.cpp), [`Editor/EditorViewport.cpp`](../../Source/Editor/EditorViewport.cpp) and the other files in `Source/Editor`

> [!WARNING]
> **Not built yet.** `Source/Editor` is not part of any CMake target. It uses raylib, which is not in this repository, and game types such as `GameObject`, `Player` and `SceneManagement::Scene`, which exist only in `.backup` files or not at all.

## Description

The in-game world editor: a set of Dear ImGui panels, a fly camera and viewport tools for building and inspecting a level while the game runs.

`WorldEditor` is a singleton: get it with [getInstance](#worldeditorgetinstance). Press **F1** to open or close it. While it is open, the [EditorCamera](EditorCamera.md) drives the view, the physics simulation is paused by default, and you can select, move, rotate, scale, place, duplicate and delete objects. The editor does nothing in production builds.

### Viewport

Mouse input in the viewport goes to the first of these that wants it, in this order:

1. **ImGui.** A click on an editor window never reaches the world behind it.
2. **Camera.** While the right mouse button is held, the mouse only looks around.
3. **Placement.** While point-and-place is armed, a left click drops the staged object on the surface under the cursor. While picking a target for a mini-game station, a left click sets that target.
4. **Gizmo.** The [EditorGizmo](EditorGizmo.md) handles on the selected object.
5. **Selection.** A left click selects the object under the cursor (see [PickResult](PickResult.md)).

### Shortcuts

Shortcuts are ignored while an ImGui text field has the keyboard.

| Key | Action |
| --- | --- |
| F1 | Open or close the editor. |
| 1 / 2 / 3 / 4 | Select / Move / Rotate / Scale tool. |
| X | Toggle local or world space for the gizmo. |
| G | Toggle snapping. |
| P | Arm or disarm point-and-place (opens the Placement panel). |
| F | Focus the camera on the selection. |
| Delete | Delete the selection. |
| Ctrl+Z | Undo. |
| Ctrl+D | Duplicate the selection. |
| Ctrl+1 … Ctrl+8 | Toggle World Settings, Object Browser, Placement, Player Data, Camera Data, Mini Game Data, Asset Data, Lighting. |

### Panels

The **World Editor** hub is always shown while the editor is open. It holds the transform tools (tool, space, snapping, pause simulation, undo, duplicate, delete, focus, snap to grid), checkboxes for every other panel, and a status line. The other panels are World Settings (open by default), Object Browser, Placement, Player Data, Camera Data, Mini Game Data, Asset Data, Lighting, Stalker AI and UI Data. Panel visibility is remembered while the editor is closed.

### Undo

Gizmo drags that changed something, **Snap To Grid**, point-and-place and duplicating can be undone; the last 128 steps are kept as [EditorEdit](EditorEdit.md) records. Deleting cannot be undone. **Load Game**, **New World** and **Load World** clear the selection and the history, because ids from the old world mean nothing in the new one.

```cpp
// Game loop integration
Editor::WorldEditor& editor = Editor::WorldEditor::getInstance();

// Update
editor.update(scene);
if (!(editor.IsEnabled() && editor.IsSimulationPaused())) {
    StepPhysics(scene, dt);
}
ui.SetMouseBlocked(editor.CapturesMouse());

// Draw, inside BeginMode3D
DrawScene(scene);
editor.DrawViewport3D(scene);
```

`WorldEditor` cannot be copied or moved.

Additional resources: [EditorCamera](EditorCamera.md), [EditorGizmo](EditorGizmo.md), [EditorEdit](EditorEdit.md), [PickResult](PickResult.md).

## Static Methods

| Method | Description |
| --- | --- |
| [getInstance](#worldeditorgetinstance) | Returns the one world editor. |

## Properties

| Property | Description |
| --- | --- |
| [editorCamera](#worldeditoreditorcamera) | The fly camera used while the editor is open. |

## Public Methods

| Method | Description |
| --- | --- |
| [update](#worldeditorupdate) | Runs the editor for one frame. |
| [DrawViewport3D](#worldeditordrawviewport3d) | Draws the selection outline, gizmo and placement preview. |
| [IsEnabled](#worldeditorisenabled) | Whether the editor is open. |
| [CapturesMouse](#worldeditorcapturesmouse) | Whether an editor window has the mouse this frame. |
| [IsSimulationPaused](#worldeditorissimulationpaused) | Whether the editor wants physics paused. |
| [SelectObject](#worldeditorselectobject) | Selects an object by id. |

---

## WorldEditor.getInstance

### Declaration

```cpp
static WorldEditor& getInstance();
```

### Returns

**WorldEditor&** The one world editor.

### Description

Returns the one world editor, creating it on first use. It starts closed.

---

## WorldEditor.editorCamera

### Declaration

```cpp
EditorCamera editorCamera = {};
```

### Description

The fly camera used while the editor is open. See [EditorCamera](EditorCamera.md).

---

## WorldEditor.update

### Declaration

```cpp
void update(SceneManagement::Scene& scene);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `scene` | The scene to edit this frame. |

### Description

Runs the editor for one frame: the F1 toggle, shortcuts, viewport input and every open panel.

When F1 opens the editor, the [editorCamera](#worldeditoreditorcamera) adopts the scene camera's pose so the view does not jump. When F1 closes it, any gizmo drag is cancelled and point-and-place is disarmed. While the editor is closed, `update` only watches for F1.

The editor holds on to `scene` only for the duration of the call, so nothing dangles if the scene is replaced. In production builds `update` returns immediately.

---

## WorldEditor.DrawViewport3D

### Declaration

```cpp
void DrawViewport3D(SceneManagement::Scene& scene);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `scene` | The scene being edited. |

### Description

Draws the editor's 3D overlays. Call it inside raylib's `BeginMode3D`, after the scene itself. Draws nothing while the editor is closed.

It draws, without depth testing so they show through other geometry:

* A wire outline around the selected object, following its rotation.
* For a selected mini-game station, a second outline around the object it targets.
* The [EditorGizmo](EditorGizmo.md) handles, unless point-and-place is armed.
* While point-and-place is armed, a translucent preview box where the object will land, and a short line showing the surface direction it will be pushed off.

---

## WorldEditor.IsEnabled

### Declaration

```cpp
bool IsEnabled() const;
```

### Returns

**bool** `true` while the editor is open.

### Description

Whether the editor is open.

---

## WorldEditor.CapturesMouse

### Declaration

```cpp
bool CapturesMouse() const;
```

### Returns

**bool** `true` when the editor is open and one of its windows is under the mouse this frame.

### Description

Whether an editor window has the mouse this frame. The game's own UI is drawn underneath the editor, so it should ignore the mouse while this is `true`; otherwise a click on an inspector would also press the menu button behind it.

---

## WorldEditor.IsSimulationPaused

### Declaration

```cpp
bool IsSimulationPaused() const;
```

### Returns

**bool** The state of the **Pause Simulation** checkbox. `true` by default.

### Description

Whether the editor wants physics paused. The scene asks this before stepping physics. It is only meaningful while the editor is open, so check [IsEnabled](#worldeditorisenabled) too.

Paused is the default because, with physics running, gravity drags newly placed objects away and the solver fights every gizmo drag.

---

## WorldEditor.SelectObject

### Declaration

```cpp
void SelectObject(std::uint64_t id);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `id` | Id of the object to select, or 0 to clear the selection. |

### Description

Selects an object by id. The editor stores the id rather than a pointer and looks the object up each frame, so a selection never points at a destroyed object.
