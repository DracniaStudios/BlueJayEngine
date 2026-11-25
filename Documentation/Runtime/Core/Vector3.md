# Vector3

struct in global namespace / Implemented in: [`Core/Core.h`](../../../Source/Runtime/Core/Core.h)

## Description

A 3D vector or point with `float` components.

`Vector3` is used for positions, directions and colours throughout the engine. All three components default to `0`. When used as a colour (for example [Triangle.color](Triangle.md#trianglecolor)), `x`, `y` and `z` are red, green and blue in the range 0 to 1.

The engine uses a right-handed coordinate system. The [Camera](../Rendering/Camera.md) looks down -Z by default, +Y is up.

```cpp
#include <Core/Core.h>

Vector3 a{ 1, 0, 0 };
Vector3 b{ 0, 1, 0 };

Vector3 sum    = a + b;               // { 1, 1, 0 }
Vector3 scaled = sum * 2.0f;          // { 2, 2, 0 }
float   d      = DotProduct(a, b);    // 0
Vector3 n      = CrossProduct(a, b);  // { 0, 0, 1 }
Vector3 dir    = Normalize(scaled);   // { 0.707, 0.707, 0 }
```

Additional resources: [Vector2](Vector2.md), [Matrix4x4](Matrix4x4.md).

## Properties

| Property | Description |
| --- | --- |
| [x](#vector3x) | X component of the vector. |
| [y](#vector3y) | Y component of the vector. |
| [z](#vector3z) | Z component of the vector. |

## Operators

| Operator | Description |
| --- | --- |
| [operator+](#vector3-operators) | Adds a vector or a scalar to every component. |
| [operator-](#vector3-operators) | Subtracts a vector or a scalar from every component. |
| [operator*](#vector3-operators) | Multiplies by a scalar, or by a vector component by component. |
| [operator/](#vector3-operators) | Divides by a scalar, or by a vector component by component. |

## Related Functions

These free functions are declared next to `Vector3` in `Core.h`.

| Function | Description |
| --- | --- |
| [DotProduct](#dotproduct) | Dot product of two vectors. |
| [CrossProduct](#crossproduct) | Cross product of two vectors. |
| [Normalize](#normalize) | Returns the vector scaled to length 1. |

---

## Vector3.x

### Declaration

```cpp
float x = 0;
```

### Description

X component of the vector. Red when the vector is a colour.

---

## Vector3.y

### Declaration

```cpp
float y = 0;
```

### Description

Y component of the vector. Green when the vector is a colour.

---

## Vector3.z

### Declaration

```cpp
float z = 0;
```

### Description

Z component of the vector. Blue when the vector is a colour.

---

## Vector3 operators

### Declaration

```cpp
Vector3 operator+(Vector3 a, Vector3 b);
Vector3 operator+(Vector3 a, float b);
Vector3 operator-(Vector3 a, Vector3 b);
Vector3 operator-(Vector3 a, float b);
Vector3 operator*(Vector3 a, float s);
Vector3 operator*(Vector3 a, Vector3 b);
Vector3 operator/(Vector3 a, Vector3 b);
Vector3 operator/(Vector3 a, float b);
```

### Description

Every operator works component by component and returns a new vector.

| Expression | Result |
| --- | --- |
| `a + b` | `{ a.x + b.x, a.y + b.y, a.z + b.z }` |
| `a + s` | `{ a.x + s, a.y + s, a.z + s }` |
| `a - b` | `{ a.x - b.x, a.y - b.y, a.z - b.z }` |
| `a - s` | `{ a.x - s, a.y - s, a.z - s }` |
| `a * s` | `{ a.x * s, a.y * s, a.z * s }` |
| `a * b` | `{ a.x * b.x, a.y * b.y, a.z * b.z }` |
| `a / b` | `{ a.x / b.x, a.y / b.y, a.z / b.z }` |
| `a / s` | `{ a.x / s, a.y / s, a.z / s }` |

The scalar always goes on the right: `v * 2.0f` compiles, `2.0f * v` does not. There is no unary minus; negate with `v * -1.0f`.

---

## DotProduct

### Declaration

```cpp
float DotProduct(Vector3 a, Vector3 b);
```

### Returns

**float** `a.x * b.x + a.y * b.y + a.z * b.z`.

### Description

Dot product of two vectors.

For unit vectors the result is the cosine of the angle between them: `1` when they point the same way, `0` when they are perpendicular and `-1` when they point in opposite directions. The [Renderer](../Rendering/Renderer.md) uses it for back-face culling and diffuse lighting.

---

## CrossProduct

### Declaration

```cpp
Vector3 CrossProduct(Vector3 a, Vector3 b);
```

### Returns

**Vector3** A vector perpendicular to both `a` and `b`.

### Description

Cross product of two vectors.

The length of the result is `|a| * |b| * sin(angle)`, and its direction follows the right-hand rule: `CrossProduct({1,0,0}, {0,1,0})` is `{0,0,1}`. The [Renderer](../Rendering/Renderer.md) uses it to find each triangle's face normal.

---

## Normalize

### Declaration

```cpp
Vector3 Normalize(Vector3 v);
```

### Returns

**Vector3** `v` scaled to length 1.

### Description

Returns the vector scaled to length 1, keeping its direction.

A zero-length vector is returned unchanged instead of producing NaN.
