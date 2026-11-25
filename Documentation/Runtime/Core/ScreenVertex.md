# ScreenVertex

struct in global namespace / Implemented in: [`Core/Core.h`](../../../Source/Runtime/Core/Core.h)

## Description

A vertex after projection to the screen: a pixel position plus a depth value.

The [Renderer](../Rendering/Renderer.md) produces screen vertices from clip-space vertices with a perspective divide and a viewport transform, then rasterises triangles of them with its depth test. Pixel (0, 0) is the top-left corner of the framebuffer and Y grows downward.

The members have no default values.

Additional resources: [Renderer](../Rendering/Renderer.md), [Matrix4x4](Matrix4x4.md).

## Properties

| Property | Description |
| --- | --- |
| [x](#screenvertexx) | Horizontal position, in pixels from the left edge. |
| [y](#screenvertexy) | Vertical position, in pixels from the top edge. |
| [z](#screenvertexz) | Depth in normalised device coordinates, from 0 (near) to 1 (far). |

---

## ScreenVertex.x

### Declaration

```cpp
float x;
```

### Description

Horizontal position, in pixels from the left edge of the framebuffer. Fractional values are kept so the rasteriser can sample pixel centres precisely.

---

## ScreenVertex.y

### Declaration

```cpp
float y;
```

### Description

Vertical position, in pixels from the top edge of the framebuffer.

---

## ScreenVertex.z

### Declaration

```cpp
float z;
```

### Description

Depth in normalised device coordinates. 0 is the near plane and 1 is the far plane (see [Perspective](Matrix4x4.md#perspective)). The [Renderer](../Rendering/Renderer.md) keeps a pixel only when its interpolated depth is less than the value already in the depth buffer.
