# Transform3D

struct in `Physics3D` / Inherits from: raylib `Transform` / Implemented in: [`Physics/Physics.h`](../../../Source/Runtime/Physics/Physics.h)

> [!WARNING]
> **Not built yet.** `Physics.h` uses raylib types (`Transform`, `Vector3`, `Quaternion`, `Rectangle`) and includes `Collider.h`. Neither raylib nor `Collider.h` is in this repository, so nothing in the engine includes this header yet. `Vector3` on this page is raylib's, not [the engine's](../Core/Vector3.md).

## Description

A position, rotation and scale in 3D, with helpers for the edges of the box it describes.

`Transform3D` adds edge, corner and bounding-rectangle helpers to raylib's `Transform`. All of them treat the transform as an axis-aligned box of size `scale` centred on `translation`, and **ignore rotation**. They also follow a screen-style vertical convention: "top" is the smaller Y value and "bottom" the larger, even though [up](#transform3dup) is +Y.

[RigidBody3D](RigidBody3D.md) derives from `Transform3D`.

```cpp
#include <Physics/Physics.h>

Physics3D::Transform3D box;
box.translation = { 0, 2, 0 };
box.rotation = QuaternionIdentity();
box.scale = { 4, 2, 4 };

Vector3 top = box.getTop();          // { 0, 1, 0 }: smaller Y
Rectangle footprint = box.getAABB(); // { -2, 1, 4, 2 } in the XY plane
```

Additional resources: [RigidBody3D](RigidBody3D.md), [Transform2D](Transform2D.md).

## Properties

| Property | Description |
| --- | --- |
| [forward](#transform3d-direction-vectors) | Forward direction, +Z. |
| [back](#transform3d-direction-vectors) | Backward direction, -Z. |
| [right](#transform3d-direction-vectors) | Right direction, -X. |
| [left](#transform3d-direction-vectors) | Left direction, +X. |
| [up](#transform3dup) | Up direction, +Y. |
| [down](#transform3d-direction-vectors) | Down direction, -Y. |

## Public Methods

| Method | Description |
| --- | --- |
| [getPosition](#transform3dgetposition) | Reference to `translation`, for changing the position in place. |
| [getCenter](#transform3dgetcenter) | Copy of `translation`. |
| [getTop, getBottom, getLeft, getRight](#transform3d-edge-points) | Midpoints of the box's edges in the XY plane. |
| [getTopLeft, getTopRight, getBottomLeft, getBottomRight](#transform3d-corner-points) | Corners of the box in the XY plane. |
| [getAABB](#transform3dgetaabb) | The box's footprint in the XY plane, as a raylib `Rectangle`. |

## Inherited Members

### Properties (raylib `Transform`)

| Property | Description |
| --- | --- |
| `translation` | Position (`Vector3`). |
| `rotation` | Orientation (`Quaternion`). |
| `scale` | Size of the box on each axis (`Vector3`). |

---

## Transform3D.getPosition

### Declaration

```cpp
Vector3& getPosition();
```

### Returns

**Vector3&** A reference to `translation`.

### Description

Reference to `translation`, for changing the position in place: `body.getPosition().y += 1.0f;`.

---

## Transform3D.getCenter

### Declaration

```cpp
Vector3 getCenter() const;
```

### Returns

**Vector3** A copy of `translation`.

### Description

Centre of the box, which is the same as `translation`.

---

## Transform3D edge points

### Declaration

```cpp
Vector3 getTop() const;
Vector3 getBottom() const;
Vector3 getLeft() const;
Vector3 getRight() const;
```

### Description

Midpoints of the box's edges in the XY plane, at the same Z as `translation`.

| Method | Result |
| --- | --- |
| `getTop` | `translation` with Y reduced by `scale.y / 2`. |
| `getBottom` | `translation` with Y increased by `scale.y / 2`. |
| `getLeft` | `translation` with X reduced by `scale.x / 2`. |
| `getRight` | `translation` with X increased by `scale.x / 2`. |

Rotation is ignored. Note that `getLeft` and `getRight` use -X and +X, which is the opposite of the [left](#transform3d-direction-vectors) and [right](#transform3d-direction-vectors) direction vectors.

---

## Transform3D corner points

### Declaration

```cpp
Vector3 getTopLeft() const;
Vector3 getTopRight() const;
Vector3 getBottomLeft() const;
Vector3 getBottomRight() const;
```

### Description

Corners of the box in the XY plane, at the same Z as `translation`. "Top" is the smaller Y and "left" the smaller X, so `getTopLeft` is `translation - scale / 2` on X and Y. Rotation is ignored.

---

## Transform3D.getAABB

### Declaration

```cpp
Rectangle getAABB() const;
```

### Returns

**Rectangle** `{ translation.x - scale.x / 2, translation.y - scale.y / 2, scale.x, scale.y }`.

### Description

The box's footprint in the XY plane, as a raylib `Rectangle` whose `x` and `y` are its top-left (smallest) corner. Z and rotation are ignored.

---

## Transform3D.up

### Declaration

```cpp
Vector3 up = Vector3(0, 1, 0);
```

### Description

Up direction, +Y. See [direction vectors](#transform3d-direction-vectors).

---

## Transform3D direction vectors

### Declaration

```cpp
Vector3 forward = Vector3(0, 0, 1);
Vector3 back    = Vector3(0, 0, -1);
Vector3 right   = Vector3(-1, 0, 0);
Vector3 left    = Vector3(1, 0, 0);
Vector3 up      = Vector3(0, 1, 0);
Vector3 down    = Vector3(0, -1, 0);
```

### Description

The six axis directions this module uses. +Z is forward and right is `cross(forward, up)`, which makes right **-X**. The source notes that the game's player and editor cameras build their right and left vectors with the same convention.

These are stored values with fixed defaults. `Transform3D` does not update them when `rotation` changes.
