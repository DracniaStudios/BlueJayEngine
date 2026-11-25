# Matrix4x4

struct in global namespace / Implemented in: [`Core/Core.h`](../../../Source/Runtime/Core/Core.h)

## Description

A 4x4 transformation matrix.

The matrix is stored row-major in `m[row][column]` and uses the column-vector convention: a point is transformed as `p' = M * p`. When you combine matrices, the transform on the right is applied first. `Translation(t) * RotationY(a)` rotates the point and then moves it.

A default-constructed `Matrix4x4` is the identity matrix.

Use the related functions below to build model, view and projection matrices, and [TransformPoint](#transformpoint) to apply them.

```cpp
#include <Core/Core.h>

const float pi = 3.14159265f;

Matrix4x4 model = Translation({ 0, 0, -5 }) * RotationY(pi * 0.25f);
Matrix4x4 proj  = Perspective(60.0f * pi / 180.0f, 16.0f / 9.0f, 0.1f, 100.0f);

float w;
Vector3 clip = TransformPoint(proj * model, { 1, 1, 1 }, &w);
Vector3 ndc  = clip / w;   // perspective divide
```

Additional resources: [Vector3](Vector3.md), [Camera.View](../Rendering/Camera.md#cameraview), [Renderer.DrawMesh](../Rendering/Renderer.md#rendererdrawmesh).

## Properties

| Property | Description |
| --- | --- |
| [m](#matrix4x4m) | The 16 matrix elements, indexed `m[row][column]`. |

## Operators

| Operator | Description |
| --- | --- |
| [operator*](#matrix4x4-operator) | Multiplies two matrices. |

## Related Functions

| Function | Description |
| --- | --- |
| [Translation](#translation) | Matrix that moves points by an offset. |
| [RotationX](#rotationx) | Matrix that rotates points around the X axis. |
| [RotationY](#rotationy) | Matrix that rotates points around the Y axis. |
| [Perspective](#perspective) | Right-handed perspective projection matrix. |
| [TransformPoint](#transformpoint) | Applies a matrix to a point. |

There is no `RotationZ` yet; `Core.h` has a placeholder comment where it will go.

---

## Matrix4x4.m

### Declaration

```cpp
float m[4][4];   // defaults to the identity matrix
```

### Description

The 16 matrix elements, indexed `m[row][column]`.

The translation of an affine transform is in the last column: `m[0][3]`, `m[1][3]` and `m[2][3]`.

---

## Matrix4x4 operator*

### Declaration

```cpp
Matrix4x4 operator*(const Matrix4x4& a, const Matrix4x4& b);
```

### Returns

**Matrix4x4** The product `a * b`.

### Description

Multiplies two matrices.

Transforming a point by `a * b` gives the same result as transforming it by `b` and then by `a`.

---

## Translation

### Declaration

```cpp
Matrix4x4 Translation(Vector3 t);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `t` | Offset to move points by. |

### Returns

**Matrix4x4** An identity matrix with `t` in the last column.

### Description

Matrix that moves points by an offset.

---

## RotationX

### Declaration

```cpp
Matrix4x4 RotationX(float a);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `a` | Rotation angle, in radians. |

### Returns

**Matrix4x4** A rotation of `a` radians around the X axis.

### Description

Matrix that rotates points around the X axis.

A positive angle turns +Y toward +Z (counter-clockwise when you look down the X axis toward the origin).

---

## RotationY

### Declaration

```cpp
Matrix4x4 RotationY(float a);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `a` | Rotation angle, in radians. |

### Returns

**Matrix4x4** A rotation of `a` radians around the Y axis.

### Description

Matrix that rotates points around the Y axis.

A positive angle turns +Z toward +X (counter-clockwise when you look down the Y axis toward the origin).

---

## Perspective

### Declaration

```cpp
Matrix4x4 Perspective(float fovY, float aspect, float zn, float zf);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `fovY` | Vertical field of view, in radians. |
| `aspect` | Width divided by height of the viewport. |
| `zn` | Distance to the near clipping plane. Must be greater than 0. |
| `zf` | Distance to the far clipping plane. Must be greater than `zn`. |

### Returns

**Matrix4x4** A perspective projection matrix.

### Description

Right-handed perspective projection matrix.

The camera looks down -Z. Points on the near plane map to clip depth 0 and points on the far plane map to clip depth 1, the same convention as Direct3D and Vulkan. The resulting `w` is the view-space distance in front of the camera, so divide `x`, `y` and `z` by `w` after transforming to get normalised device coordinates.

The [Renderer](../Rendering/Renderer.md) clips triangles against the near plane that this matrix maps to depth 0.

---

## TransformPoint

### Declaration

```cpp
Vector3 TransformPoint(const Matrix4x4& M, Vector3 p, float* wOut = nullptr);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `M` | Matrix to apply. |
| `p` | Point to transform. It is treated as `(p.x, p.y, p.z, 1)`. |
| `wOut` | Optional. Receives the transformed `w` component. |

### Returns

**Vector3** The transformed `x`, `y` and `z`, **without** a perspective divide.

### Description

Applies a matrix to a point.

For affine matrices (translation and rotation) `w` stays 1 and you can ignore it. For a projection matrix, pass `wOut` and divide the result by it yourself.

To transform a direction instead of a point, use a matrix without translation, as [Camera.ToViewSpace](../Rendering/Camera.md#cameratoviewspace) does.
