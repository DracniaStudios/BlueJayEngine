# Color4

struct in global namespace / Implemented in: [`Core/Core.h`](../../../Source/Runtime/Core/Core.h)

## Description

An RGBA colour with 8-bit-range channels.

Each channel holds a value from 0 to 255 in an `unsigned int`. The channels have no default value, so always initialise a `Color4`, for example `Color4{ 255, 128, 0, 255 }`.

Adding two colours saturates at 255. Subtracting does **not** saturate at 0; see [operators](#color4-operators).

The [Renderer](../Rendering/Renderer.md) does not use `Color4` yet. Mesh colours are [Vector3](Vector3.md) values from 0 to 1.

```cpp
#include <Core/Core.h>

Color4 base{ 200, 100, 50, 255 };
Color4 glow{ 100, 100, 100, 0 };
Color4 lit = base + glow;   // { 255, 200, 150, 255 }: red saturates at 255
```

Additional resources: [Vector3](Vector3.md).

## Properties

| Property | Description |
| --- | --- |
| [r](#color4r) | Red channel, 0 to 255. |
| [g](#color4g) | Green channel, 0 to 255. |
| [b](#color4b) | Blue channel, 0 to 255. |
| [a](#color4a) | Alpha channel, 0 to 255. |

## Operators

| Operator | Description |
| --- | --- |
| [operator+](#color4-operators) | Adds two colours channel by channel, saturating at 255. |
| [operator-](#color4-operators) | Subtracts one colour from another channel by channel. |

---

## Color4.r

### Declaration

```cpp
unsigned int r;
```

### Description

Red channel, 0 to 255.

---

## Color4.g

### Declaration

```cpp
unsigned int g;
```

### Description

Green channel, 0 to 255.

---

## Color4.b

### Declaration

```cpp
unsigned int b;
```

### Description

Blue channel, 0 to 255.

---

## Color4.a

### Declaration

```cpp
unsigned int a;
```

### Description

Alpha channel, 0 to 255. 255 is fully opaque.

---

## Color4 operators

### Declaration

```cpp
Color4 operator+(const Color4 a, const Color4 b);
Color4 operator-(const Color4 a, const Color4 b);
```

### Description

Both operators work channel by channel, including alpha, and clamp each result to at most 255.

`a + b` saturates as expected: any channel that would exceed 255 becomes 255.

`a - b` is meant to clamp the same way, but the channels are unsigned. When a channel of `b` is larger than the matching channel of `a`, the subtraction wraps around to a very large number, which is then clamped to **255**, not 0. For example, `Color4{ 10, 10, 10, 255 } - Color4{ 20, 0, 0, 0 }` gives a red channel of 255. Make sure each channel of `b` is no larger than the one in `a`, or compare channels yourself.
