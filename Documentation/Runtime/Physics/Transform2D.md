# Transform2D

struct in `Physics2D` / Inherits from: raylib `Transform` / Implemented in: [`Physics/Physics.h`](../../../Source/Runtime/Physics/Physics.h)

> [!WARNING]
> **Not built yet.** `Physics.h` uses raylib types (`Transform`, `Vector2`, `Rectangle`) and includes `Collider.h`. Neither raylib nor `Collider.h` is in this repository, so nothing in the engine includes this header yet. `Vector2` on this page is raylib's, not [the engine's](../Core/Vector2.md).

## Description

A position and size in 2D, with edge helpers and rectangle overlap tests.

`Transform2D` reuses raylib's 3D `Transform` and reads only the X and Y parts of `translation` and `scale`. The transform is treated as an axis-aligned rectangle of size `scale` centred on `translation`. **Rotation is ignored.**

It uses screen-style coordinates: +Y is down, so "top" is the smaller Y value.

[RigidBody2D](RigidBody2D.md) derives from `Transform2D`.

```cpp
#include <Physics/Physics.h>

Physics2D::Transform2D button;
button.translation = { 100, 50, 0 };
button.scale = { 80, 20, 1 };

Vector2 mouse = GetMousePosition();
if (button.intersectPoint(mouse, 2.0f)) {   // 2-pixel grace margin
    // hovered
}
```

Additional resources: [RigidBody2D](RigidBody2D.md), [Transform3D](Transform3D.md).

## Public Methods

| Method | Description |
| --- | --- |
| [getCenter](#transform2dgetcenter) | The X and Y of `translation`. |
| [getTop, getBottom, getLeft, getRight](#transform2d-edge-points) | Midpoints of the rectangle's edges. |
| [getTopLeft, getTopRight, getBottomLeft, getBottomRight](#transform2d-corner-points) | Corners of the rectangle. |
| [getAABB](#transform2dgetaabb) | The rectangle as a raylib `Rectangle`. |
| [intersectPoint](#transform2dintersectpoint) | Whether a point is inside the rectangle. |
| [intersectTransform](#transform2dintersecttransform) | Whether two rectangles overlap. |

## Inherited Members

### Properties (raylib `Transform`)

| Property | Description |
| --- | --- |
| `translation` | Position. Only `x` and `y` are used. |
| `rotation` | Not used. |
| `scale` | Size. Only `x` and `y` are used. |

---

## Transform2D.getCenter

### Declaration

```cpp
Vector2 getCenter() const;
```

### Returns

**Vector2** `{ translation.x, translation.y }`.

### Description

Centre of the rectangle.

---

## Transform2D edge points

### Declaration

```cpp
Vector2 getTop() const;
Vector2 getBottom() const;
Vector2 getLeft() const;
Vector2 getRight() const;
```

### Description

Midpoints of the rectangle's edges. `getTop` is `scale.y / 2` above the centre (smaller Y), `getBottom` the same distance below, and `getLeft` and `getRight` are `scale.x / 2` to either side.

---

## Transform2D corner points

### Declaration

```cpp
Vector2 getTopLeft() const;
Vector2 getTopRight() const;
Vector2 getBottomLeft() const;
Vector2 getBottomRight() const;
```

### Description

Corners of the rectangle. `getTopLeft` is the corner with the smallest X and Y.

---

## Transform2D.getAABB

### Declaration

```cpp
Rectangle getAABB() const;
```

### Returns

**Rectangle** `{ translation.x - scale.x / 2, translation.y - scale.y / 2, scale.x, scale.y }`.

### Description

The rectangle as a raylib `Rectangle`, positioned by its top-left corner.

---

## Transform2D.intersectPoint

### Declaration

```cpp
bool intersectPoint(Vector2 point, float delta = 0) const;
```

### Parameters

| Parameter | Description |
| --- | --- |
| `point` | The point to test. |
| `delta` | Margin added to every side of the rectangle before testing. Negative values shrink it. |

### Returns

**bool** `true` if `point` is inside the rectangle grown by `delta`.

### Description

Whether a point is inside the rectangle, with an optional margin.

---

## Transform2D.intersectTransform

### Declaration

```cpp
bool intersectTransform(Transform2D other, float delta = 0) const;
```

### Parameters

| Parameter | Description |
| --- | --- |
| `other` | The other transform. |
| `delta` | Margin added to every side of **both** rectangles before testing. |

### Returns

**bool** `true` if the two rectangles, each grown by `delta`, overlap.

### Description

Whether two rectangles overlap. Because both are grown, two rectangles up to `2 * delta` apart count as overlapping.
