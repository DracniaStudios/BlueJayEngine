# Vector2

struct in global namespace / Implemented in: [`Core/Core.h`](../../../Source/Runtime/Core/Core.h)

## Description

A 2D vector or point with `float` components.

Both components default to `0`, so `Vector2{}` is the zero vector. All operators work component by component.

```cpp
#include <Core/Core.h>

Vector2 position{ 10.0f, 4.0f };
Vector2 velocity{ 2.0f, -1.0f };
position = position + velocity;          // { 12, 3 }
Vector2 half = position / Vector2{ 2, 2 }; // { 6, 1.5 }
```

Additional resources: [Vector3](Vector3.md).

## Properties

| Property | Description |
| --- | --- |
| [x](#vector2x) | X component of the vector. |
| [y](#vector2y) | Y component of the vector. |

## Operators

| Operator | Description |
| --- | --- |
| [operator+](#vector2-operators) | Adds two vectors component by component. |
| [operator-](#vector2-operators) | Subtracts one vector from another component by component. |
| [operator*](#vector2-operators) | Multiplies two vectors component by component. |
| [operator/](#vector2-operators) | Divides one vector by another component by component. |

---

## Vector2.x

### Declaration

```cpp
float x = 0;
```

### Description

X component of the vector.

---

## Vector2.y

### Declaration

```cpp
float y = 0;
```

### Description

Y component of the vector.

---

## Vector2 operators

### Declaration

```cpp
Vector2 operator+(Vector2 a, Vector2 b);
Vector2 operator-(Vector2 a, Vector2 b);
Vector2 operator*(Vector2 a, Vector2 b);
Vector2 operator/(Vector2 a, Vector2 b);
```

### Description

Each operator applies to `x` and `y` separately and returns a new vector: `a + b` is `{ a.x + b.x, a.y + b.y }`, and the same pattern holds for `-`, `*` and `/`.

There are no scalar overloads. To scale a `Vector2`, multiply by a vector with the same value in both components, for example `v * Vector2{ 2, 2 }`. Dividing by a vector with a zero component follows IEEE float rules (infinity or NaN).
