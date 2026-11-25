# Renderer

class in global namespace / Implemented in: [`Rendering/Renderer.h`](../../../Source/Runtime/Rendering/Renderer.h)

## Description

A software rasteriser that draws flat-shaded meshes into a 32-bit framebuffer on the CPU.

The renderer owns a colour buffer and a depth buffer of the size you construct it with. Each frame you [Clear](#rendererclear) it, draw meshes with [DrawMesh](#rendererdrawmesh), and hand [Pixels](#rendererpixels) to a [Window](Window.md) to show them. [Run](../Application/Game.md#run) creates the renderer and presents it for you; your [Game.Render](../Application/Game.md#gamerender) only clears and draws.

`DrawMesh` does the following for every triangle:

* **Back-face culling.** Triangles that face away from the camera are skipped, which is why [Triangle](../Core/Triangle.md) indices must be counter-clockwise as seen from outside.
* **Flat lighting.** One colour per triangle: 20% ambient plus 80% diffuse from a single directional light.
* **Near-plane clipping.** Triangles that cross the near plane are cut against it, so geometry passing the camera is drawn correctly instead of being dropped.
* **Depth testing.** A pixel is written only when it is nearer than what is already there.

Colours are converted to 8-bit with an approximate gamma of 2.0, so [Triangle.color](../Core/Triangle.md#trianglecolor) is in linear space.

```cpp
void MyGame::Render(Renderer& renderer) {
    const Matrix4x4 view = camera_.View();
    const Vector3 lightDir = camera_.ToViewSpace(Normalize({ 0.4f, 0.8f, 0.6f }));

    renderer.Clear(0xFF14181F);   // dark blue-grey, opaque
    renderer.DrawMesh(cube_, Translation({ -2, 0, 0 }) * RotationY(angle_), view, proj_, lightDir);
    renderer.DrawMesh(pyramid_, Translation({ 2, 0, 0 }), view, proj_, lightDir);
}
```

`Renderer` cannot be copied, because it owns an [Arena](../Core/Arena.md).

Additional resources: [Mesh](../Core/Mesh.md), [Camera](Camera.md), [Window](Window.md), [Matrix4x4](../Core/Matrix4x4.md).

## Constructors

| Constructor | Description |
| --- | --- |
| [Renderer](#rendererrenderer) | Creates colour and depth buffers of the given size. |

## Public Methods

| Method | Description |
| --- | --- |
| [Clear](#rendererclear) | Fills the frame with one colour and resets depth and scratch memory. |
| [DrawMesh](#rendererdrawmesh) | Draws a mesh with a model, view and projection matrix. |
| [Pixels](#rendererpixels) | The colour buffer, ready to present. |

---

## Renderer.Renderer

### Declaration

```cpp
Renderer(int w, int h);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `w` | Width of the framebuffer, in pixels. |
| `h` | Height of the framebuffer, in pixels. |

### Description

Creates colour and depth buffers of `w` x `h` pixels, and a 1 MiB [Arena](../Core/Arena.md) for per-frame scratch memory.

The buffers start zeroed, so call [Clear](#rendererclear) before the first draw. To present with a [Window](Window.md), use `SCREEN_WIDTH` x `SCREEN_HEIGHT`, which is the size [Window.Present](Window.md#windowpresent) expects.

---

## Renderer.Clear

### Declaration

```cpp
void Clear(uint32_t argb);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `argb` | Background colour as `0xAARRGGBB`. Use `0xFF` for alpha. |

### Description

Fills the frame with one colour, resets every depth value to the far plane (1.0), and resets the scratch arena.

Call it at the start of every frame. Besides clearing the image, it frees the scratch memory that [DrawMesh](#rendererdrawmesh) allocated during the previous frame.

---

## Renderer.DrawMesh

### Declaration

```cpp
void DrawMesh(const Mesh& mesh, const Matrix4x4& model,
              const Matrix4x4& view, const Matrix4x4& proj, Vector3 lightDir);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `mesh` | The geometry to draw. |
| `model` | Places the mesh in the world (model space to world space). |
| `view` | World space to view space, usually [Camera.View](Camera.md#cameraview). |
| `proj` | View space to clip space, usually from [Perspective](../Core/Matrix4x4.md#perspective). |
| `lightDir` | Unit vector **toward** the light, in **view** space. |

### Description

Draws a mesh with a model, view and projection matrix.

Each triangle is lit by `max(0, N · lightDir)`, where `N` is the triangle's view-space face normal, so it reaches full brightness when it faces the light and drops to the 20% ambient level when it faces away. Because `lightDir` is in view space, a constant value makes the light move with the camera. To keep the light fixed in the world, convert a world-space direction each frame with [Camera.ToViewSpace](Camera.md#cameratoviewspace).

Each call allocates 16 bytes per vertex of `mesh` from the 1 MiB scratch arena, which [Clear](#rendererclear) resets. That allows about 65,000 vertices per frame in total, across all calls. Going over throws `std::bad_alloc`.

---

## Renderer.Pixels

### Declaration

```cpp
const uint32_t* Pixels() const;
```

### Returns

**const uint32_t\*** The colour buffer: `w * h` pixels in `0xAARRGGBB` format, row by row from the top-left corner.

### Description

The colour buffer, ready to present with [Window.Present](Window.md#windowpresent). The pointer stays valid for the renderer's lifetime.

---

## Implementation notes

`DrawMesh` stores each mesh's clip-space vertices as [ClipVertex](ClipVertex.md) values in the scratch arena, clips each triangle against the near plane (which can turn it into two), divides by `w`, and fills the result with an edge-function rasteriser that samples pixel centres.
