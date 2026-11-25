# ClipVertex

struct in an anonymous namespace / Implemented in: [`Rendering/Renderer.cpp`](../../../Source/Runtime/Rendering/Renderer.cpp)

> [!NOTE]
> Internal type. `ClipVertex` is private to `Renderer.cpp` and cannot be used outside it. It is documented to explain how [Renderer.DrawMesh](Renderer.md#rendererdrawmesh) works.

## Description

A vertex in homogeneous clip space, before the perspective divide.

[Renderer.DrawMesh](Renderer.md#rendererdrawmesh) transforms every vertex of a mesh by `proj * view * model` into a `ClipVertex` and stores them in the renderer's scratch [Arena](../Core/Arena.md). Triangles are clipped against the near plane in this space, where clipping is a straight-line interpolation, and only then divided by `w` to become [ScreenVertex](../Core/ScreenVertex.md) values.

Additional resources: [Renderer](Renderer.md), [ScreenVertex](../Core/ScreenVertex.md), [Perspective](../Core/Matrix4x4.md#perspective).

## Properties

| Property | Description |
| --- | --- |
| [x](#clipvertex-properties) | Clip-space X. |
| [y](#clipvertex-properties) | Clip-space Y. |
| [z](#clipvertex-properties) | Clip-space depth. 0 on the near plane. |
| [w](#clipvertex-properties) | Homogeneous W: the view-space distance in front of the camera. |

## Related Functions

| Function | Description |
| --- | --- |
| [LerpClip](#lerpclip) | Interpolates between two clip-space vertices. |
| [ClipNear](#clipnear) | Clips a triangle against the near plane. |

---

## ClipVertex properties

### Declaration

```cpp
struct ClipVertex { float x, y, z, w; };
```

### Description

The four homogeneous coordinates produced by [TransformPoint](../Core/Matrix4x4.md#transformpoint) with a projection matrix. A vertex is on the visible side of the near plane when `z >= 0`, which also means `w > 0`. Dividing `x`, `y` and `z` by `w` gives normalised device coordinates.

---

## LerpClip

### Declaration

```cpp
ClipVertex LerpClip(const ClipVertex& a, const ClipVertex& b, float t);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `a` | Start vertex, returned when `t` is 0. |
| `b` | End vertex, returned when `t` is 1. |
| `t` | Interpolation factor. |

### Returns

**ClipVertex** `a + (b - a) * t`, for all four components.

### Description

Interpolates between two clip-space vertices. Interpolating before the perspective divide keeps the result correct in perspective.

---

## ClipNear

### Declaration

```cpp
int ClipNear(const ClipVertex in[3], ClipVertex out[4]);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `in` | The triangle's three vertices. |
| `out` | Receives the clipped polygon, in the same winding order. |

### Returns

**int** Number of vertices written to `out`:

| Corners in front of the near plane | Result |
| --- | --- |
| 0 | `0`: nothing to draw. |
| 1 | `3`: a smaller triangle. |
| 2 | `4`: the corner behind the plane is cut off, leaving a quad. |
| 3 | `3`: the triangle unchanged. |

### Description

Clips a triangle against the near plane with the Sutherland-Hodgman algorithm. The caller draws the result as a fan of triangles from `out[0]`, so a 4-vertex result becomes two triangles.
