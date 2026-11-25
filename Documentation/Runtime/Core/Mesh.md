# Mesh

struct in global namespace / Implemented in: [`Core/Core.h`](../../../Source/Runtime/Core/Core.h)

## Description

Triangle geometry for the [Renderer](../Rendering/Renderer.md): a list of vertex positions and a list of flat-coloured triangles that index into it.

Vertices are in model space. Pass a model matrix to [Renderer.DrawMesh](../Rendering/Renderer.md#rendererdrawmesh) to place the mesh in the world. Each [Triangle](Triangle.md) lists its vertices counter-clockwise as seen from outside the mesh.

`Core.h` includes factory functions for a cube and a pyramid, each with a default colour scheme or with a colour per face.

```cpp
#include <Core/Core.h>
#include <Rendering/Renderer.h>

const Mesh cube    = MakeCube();
const Mesh pyramid = MakePyramid();

void Draw(Renderer& renderer, const Matrix4x4& view, const Matrix4x4& proj, Vector3 lightDir) {
    renderer.DrawMesh(cube,    Translation({ -2, 0, 0 }), view, proj, lightDir);
    renderer.DrawMesh(pyramid, Translation({  2, 0, 0 }), view, proj, lightDir);
}
```

Additional resources: [Triangle](Triangle.md), [Renderer](../Rendering/Renderer.md).

## Properties

| Property | Description |
| --- | --- |
| [vertices](#meshvertices) | Vertex positions in model space. |
| [tris](#meshtris) | Triangles that index into `vertices`. |

## Related Functions

| Function | Description |
| --- | --- |
| [MakeCube](#makecube) | Builds a cube from -1 to 1 on every axis. |
| [MakePyramid](#makepyramid) | Builds a square-based pyramid. |

---

## Mesh.vertices

### Declaration

```cpp
std::vector<Vector3> vertices;
```

### Description

Vertex positions in model space. A vertex can be shared by any number of triangles.

---

## Mesh.tris

### Declaration

```cpp
std::vector<Triangle> tris;
```

### Description

Triangles that index into [vertices](#meshvertices), each listed counter-clockwise as seen from outside the mesh. Every index must be a valid position in `vertices`.

---

## MakeCube

### Declaration

```cpp
Mesh MakeCube();
Mesh MakeCube(const Vector3 zp, const Vector3 zm,
              const Vector3 xp, const Vector3 xm,
              const Vector3 yp, const Vector3 ym);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `zp` | Colour of the +Z face. |
| `zm` | Colour of the -Z face. |
| `xp` | Colour of the +X face. |
| `xm` | Colour of the -X face. |
| `yp` | Colour of the +Y face. |
| `ym` | Colour of the -Y face. |

### Returns

**Mesh** A cube with 8 vertices and 12 triangles.

### Description

Builds a cube from -1 to 1 on every axis, so its edges are 2 units long and it is centred on the origin.

Each face is two triangles of one colour. Without arguments the faces are red (+Z), green (-Z), blue (+X), yellow (-X), magenta (+Y) and cyan (-Y).

---

## MakePyramid

### Declaration

```cpp
Mesh MakePyramid();
Mesh MakePyramid(const Vector3 zp, const Vector3 zm,
                 const Vector3 xp, const Vector3 xm,
                 const Vector3 ym);
```

### Parameters

The parameter names do not match the faces they colour. This table lists the face each parameter actually colours:

| Parameter | Face it colours |
| --- | --- |
| `zp` | -Z side. |
| `zm` | +X side. |
| `xp` | +Z side. |
| `xm` | -X side. |
| `ym` | Square base (-Y). |

### Returns

**Mesh** A pyramid with 5 vertices and 6 triangles.

### Description

Builds a square-based pyramid. The base is a 2 x 2 square at `y = -1`, centred on the Y axis, and the apex is at `(0, 1.2, 0)`.

The base is two triangles and each side is one. Without arguments the base is white and all four sides are orange.
