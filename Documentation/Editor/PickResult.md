# PickResult

struct in global namespace / Implemented in: [`Editor/EditorPicking.h`](../../Source/Editor/EditorPicking.h)

> [!WARNING]
> **Not built yet.** `Source/Editor` is not part of any CMake target. It uses raylib, which is not in this repository, and game types such as `GameObject` and `SceneManagement::Scene`, which exist only in `.backup` files or not at all.

## Description

The result of firing a mouse ray into the scene: what was hit, where, and which way the surface faces.

[PickSceneObject](#picksceneobject) returns a `PickResult` for the nearest object along a ray. The [WorldEditor](WorldEditor.md) uses it to select objects and to find where point-and-place should drop a new one.

Keep [id](#pickresultid), not [object](#pickresultobject), if you need the result beyond the current frame. The pointer becomes invalid as soon as that object is destroyed.

`EditorPicking.h` also holds the ray, plane and snapping maths that the picking code and the [EditorGizmo](EditorGizmo.md) share. They are listed below as related functions.

```cpp
const Ray ray = GetEditorMouseRay(camera);

// Select what the user clicked
PickResult pick = PickSceneObject(scene, ray, PICK_SELECTABLE);
if (pick.hit) { editor.SelectObject(pick.id); }

// Find where to place a new object: on any visible surface, else on the ground plane
PickResult surface = PickSceneObject(scene, ray, PICK_SURFACE, ghostId);
Vector3 point;
if (surface.hit) { point = surface.point; }
else if (RayGroundPlane(ray, 0.0f, point)) { /* use point */ }
point = SnapVector(point, 1.0f);
```

Additional resources: [WorldEditor](WorldEditor.md), [EditorGizmo](EditorGizmo.md).

## Properties

| Property | Description |
| --- | --- |
| [hit](#pickresulthit) | Whether anything was hit. |
| [id](#pickresultid) | Id of the object hit. |
| [object](#pickresultobject) | The object hit. Valid only until it is destroyed. |
| [distance](#pickresultdistance) | Distance along the ray to the hit. |
| [point](#pickresultpoint) | World-space hit position. |
| [normal](#pickresultnormal) | Surface normal at the hit. |

## Related Types

| Type | Description |
| --- | --- |
| [PickFilter](#pickfilter) | Which objects a pick may hit. |

## Related Functions

| Function | Description |
| --- | --- |
| [GetEditorMouseRay](#geteditormouseray) | World-space ray through the mouse cursor. |
| [PickSceneObject](#picksceneobject) | Nearest object along a ray. |
| [RayOrientedBox](#rayorientedbox) | Ray against a rotated box. |
| [RayPlane](#rayplane) | Ray against a plane. |
| [RayGroundPlane](#raygroundplane) | Ray against a horizontal plane. |
| [ClosestPointOnAxis](#closestpointonaxis) | Point on a line nearest to a ray. |
| [RayAxisDistance](#rayaxisdistance) | Distance from a ray to a line segment. |
| [RayPointDistance](#raypointdistance) | Distance from a ray to a point. |
| [SnapValue, SnapVector](#snapvalue-and-snapvector) | Round to the nearest multiple of a step. |
| [MINIMUM_EDITOR_SCALE, SanitizeScale](#minimum_editor_scale-and-sanitizescale) | Keep scale positive. |
| [SafeRotation](#saferotation) | Normalise a quaternion, falling back to identity. |

---

## PickResult.hit

### Declaration

```cpp
bool hit = false;
```

### Description

Whether anything was hit. The other fields are only meaningful when this is `true`.

---

## PickResult.id

### Declaration

```cpp
std::uint64_t id = 0;
```

### Description

Id of the object hit. This is the value to keep across frames.

---

## PickResult.object

### Declaration

```cpp
GameObject* object = nullptr;
```

### Description

The object hit. It stays valid while other objects are spawned, but becomes invalid as soon as this object is destroyed. Use it within the current frame only.

---

## PickResult.distance

### Declaration

```cpp
float distance = 0.0f;
```

### Description

Distance along the ray, from its origin to the hit.

---

## PickResult.point

### Declaration

```cpp
Vector3 point = {};
```

### Description

World-space hit position.

---

## PickResult.normal

### Declaration

```cpp
Vector3 normal = {};
```

### Description

Surface normal at the hit, always facing back toward the ray. Point-and-place uses it to rest a new object on the surface it lands on.

---

## PickFilter

### Declaration

```cpp
enum PickFilter { PICK_SELECTABLE, PICK_SURFACE };
```

### Description

Which objects a pick may hit. Selection and placement need different sets, so they use different filters.

| Value | Description |
| --- | --- |
| `PICK_SELECTABLE` | Visible objects whose `isSelectable` is set. Use it for clicks that select. The floor is not selectable, so a designer cannot drag the world out from under themselves. |
| `PICK_SURFACE` | Any visible solid, including the floor. Use it for placement, where the floor is the most common surface to drop an object onto. |

Both filters skip objects that are disabled or not drawn. A hidden object can still be found in the Object Browser.

---

## GetEditorMouseRay

### Declaration

```cpp
Ray GetEditorMouseRay(const Camera3D& camera);
```

### Returns

**Ray** World-space ray from the camera through the mouse cursor.

### Description

World-space ray through the mouse cursor, for the camera currently driving the viewport.

---

## PickSceneObject

### Declaration

```cpp
PickResult PickSceneObject(SceneManagement::Scene* scene, const Ray& ray,
                           PickFilter filter, std::uint64_t ignoreId = 0);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `scene` | Scene to search. `nullptr` returns a miss. |
| `ray` | Ray to fire, usually from [GetEditorMouseRay](#geteditormouseray). |
| `filter` | Which objects may be hit. See [PickFilter](#pickfilter). |
| `ignoreId` | Id of one object to skip. Placement passes its preview so the preview cannot land on itself. |

### Returns

**PickResult** The nearest hit, or a result with `hit == false`.

### Description

Nearest object along a ray, across every kind of object in the scene: plain objects, entities and interactables.

Each object is tested as an oriented box built from its rigid body's position, rotation and `scale * collider.size`, so rotated objects pick from the box that is actually drawn, not from their axis-aligned broad-phase box. When two hits are at exactly the same distance, the first one found wins, so the selection does not flicker between faces in the same plane.

---

## RayOrientedBox

### Declaration

```cpp
bool RayOrientedBox(const Ray& ray, Vector3 center, Vector3 size, Quaternion rotation,
                    float& outDistance, Vector3& outNormal);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `ray` | The ray. |
| `center` | Centre of the box. |
| `size` | Full size of the box along its own axes. |
| `rotation` | Orientation of the box. A degenerate quaternion is treated as identity. |
| `outDistance` | Receives the distance to the hit. |
| `outNormal` | Receives the surface normal at the hit. |

### Returns

**bool** `true` if the ray hits the box.

### Description

Ray against a rotated box. The maths is shared with the game's collider code, so the editor and the game agree on what a box covers.

---

## RayPlane

### Declaration

```cpp
bool RayPlane(const Ray& ray, Vector3 planePoint, Vector3 planeNormal, Vector3& outPoint);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `ray` | The ray. |
| `planePoint` | Any point on the plane. |
| `planeNormal` | The plane's normal. Does not need to be unit length. |
| `outPoint` | Receives the intersection point. |

### Returns

**bool** `false` when the ray is nearly parallel to the plane or the plane is behind the ray's origin.

### Description

Ray against a plane. Near-parallel rays are refused rather than returning a huge or NaN point, which would spread through the physics of any object it was written into.

---

## RayGroundPlane

### Declaration

```cpp
bool RayGroundPlane(const Ray& ray, float planeY, Vector3& outPoint);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `ray` | The ray. |
| `planeY` | Height of the horizontal plane. |
| `outPoint` | Receives the intersection point. |

### Returns

**bool** Same as [RayPlane](#rayplane).

### Description

Ray against the horizontal plane at `planeY`. Placement falls back to it when the ray hits no object.

---

## ClosestPointOnAxis

### Declaration

```cpp
bool ClosestPointOnAxis(const Ray& ray, Vector3 origin, Vector3 axis, float& outT);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `ray` | The ray. |
| `origin` | A point on the line. |
| `axis` | Direction of the line. Normalised internally. |
| `outT` | Receives the distance from `origin` along the normalised `axis` to the closest point. |

### Returns

**bool** `false` when `axis` or the ray direction is zero, or the axis points almost straight along the ray.

### Description

Finds the point on the infinite line `origin + t * axis` that is closest to the ray. Every axis drag in the [EditorGizmo](EditorGizmo.md) is built on it. When the axis points at the camera there is no screen direction to drag along, so the function refuses instead of dividing by nearly zero.

---

## RayAxisDistance

### Declaration

```cpp
float RayAxisDistance(const Ray& ray, Vector3 origin, Vector3 axis, float length);
```

### Returns

**float** Shortest distance from the ray to the segment from `origin` to `origin + normalize(axis) * length`, or `FLT_MAX` when [ClosestPointOnAxis](#closestpointonaxis) fails.

### Description

Distance from a ray to a line segment. The gizmo uses it to tell whether the cursor is over a handle.

---

## RayPointDistance

### Declaration

```cpp
float RayPointDistance(const Ray& ray, Vector3 point);
```

### Returns

**float** Shortest distance from the ray to `point`, or `FLT_MAX` if the ray has no direction.

### Description

Distance from a ray to a point. A point behind the ray's origin is measured from the origin itself.

---

## SnapValue and SnapVector

### Declaration

```cpp
float SnapValue(float value, float step);
Vector3 SnapVector(Vector3 value, float step);
```

### Returns

**float** / **Vector3** `value` rounded to the nearest multiple of `step`, per component for `SnapVector`.

### Description

Round to the nearest multiple of a step. A `step` of 0 or less returns `value` unchanged.

---

## MINIMUM_EDITOR_SCALE and SanitizeScale

### Declaration

```cpp
inline constexpr float MINIMUM_EDITOR_SCALE = 0.01f;
Vector3 SanitizeScale(Vector3 scale);
```

### Returns

**Vector3** `scale` with every component raised to at least `MINIMUM_EDITOR_SCALE`.

### Description

`MINIMUM_EDITOR_SCALE` is the smallest extent any editor operation may leave on an object. A zero or negative extent turns the collision box inside out, after which the object silently becomes impossible to click and collide with. [RigidBody3D.Update](../Runtime/Physics/RigidBody3D.md#rigidbody3dupdate) only repairs a scale of exactly zero, so the editor runs every scale it writes through `SanitizeScale`.

---

## SafeRotation

### Declaration

```cpp
Quaternion SafeRotation(Quaternion rotation);
```

### Returns

**Quaternion** `rotation` normalised, or identity if it is degenerate (for example all zeros).

### Description

Normalises a rotation, falling back to identity when it is degenerate. The game's physics update repairs a zeroed quaternion, but the editor pauses the simulation while it manipulates objects. A `{0, 0, 0, 0}` quaternion would collapse every vector it rotates to the origin and break picking and the gizmo. It forwards to the collider code's own guard, so there is one definition of the repair.
