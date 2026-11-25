# Triangle

struct in global namespace / Implemented in: [`Core/Core.h`](../../../Source/Runtime/Core/Core.h)

## Description

One face of a [Mesh](Mesh.md): three vertex indices and a flat colour.

The indices point into [Mesh.vertices](Mesh.md#meshvertices). List them **counter-clockwise as seen from outside the mesh**: the [Renderer](../Rendering/Renderer.md) uses that winding to find the face normal, and culls a triangle whose normal faces away from the camera.

`a`, `b` and `c` have no default value. Always initialise them, for example with brace initialisation.

```cpp
#include <Core/Core.h>

Mesh quad;
quad.vertices = { { -1, -1, 0 }, { 1, -1, 0 }, { 1, 1, 0 }, { -1, 1, 0 } };

const Vector3 teal{ 0.2f, 0.8f, 0.7f };
quad.tris = { { 0, 1, 2, teal },     // counter-clockwise seen from +Z
              { 0, 2, 3, teal } };
```

Additional resources: [Mesh](Mesh.md), [Renderer.DrawMesh](../Rendering/Renderer.md#rendererdrawmesh).

## Properties

| Property | Description |
| --- | --- |
| [a](#trianglea) | Index of the first vertex. |
| [b](#triangleb) | Index of the second vertex. |
| [c](#trianglec) | Index of the third vertex. |
| [color](#trianglecolor) | Flat colour of the face. |

---

## Triangle.a

### Declaration

```cpp
int a;
```

### Description

Index of the first vertex in [Mesh.vertices](Mesh.md#meshvertices).

---

## Triangle.b

### Declaration

```cpp
int b;
```

### Description

Index of the second vertex in [Mesh.vertices](Mesh.md#meshvertices).

---

## Triangle.c

### Declaration

```cpp
int c;
```

### Description

Index of the third vertex in [Mesh.vertices](Mesh.md#meshvertices).

---

## Triangle.color

### Declaration

```cpp
Vector3 color;
```

### Description

Flat colour of the face.

`x`, `y` and `z` are red, green and blue in linear space, from 0 to 1. The [Renderer](../Rendering/Renderer.md) multiplies the colour by the face's lighting, clamps each channel to 0..1, and converts it to an 8-bit ARGB pixel with an approximate gamma of 2.0.
