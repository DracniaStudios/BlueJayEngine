# RigidBody2D

struct in `Physics2D` / Inherits from: [Transform2D](Transform2D.md) / Implemented in: [`Physics/Physics.h`](../../../Source/Runtime/Physics/Physics.h)

> [!WARNING]
> **Not built yet.** `Physics.h` uses raylib types and includes `Collider.h`; neither is in this repository. No `.cpp` defines the members marked *declared only*, so their descriptions come from their declarations.

## Description

A simple 2D body with velocity, acceleration, gravity, drag and touch flags.

`RigidBody2D` is a [Transform2D](Transform2D.md) with motion state. It uses screen-style coordinates, where **+Y is down**: the default gravity `(0, 20)` pulls the body down the screen, and [jump](#rigidbody2djump) gives it a negative Y velocity.

Forces add to [acceleration](#rigidbody2dacceleration). [updateForce](#rigidbody2dupdateforce) and [update](#rigidbody2dupdate) advance the body each frame.

```cpp
#include <Physics/Physics.h>

Physics2D::RigidBody2D player;
player.teleport({ 100, 100 });
player.scale = { 16, 32, 1 };
player.bounds = { 0, 0, 640, 360 };   // stay on screen

// each frame
if (player.useGravity) { player.applyGravity(); }
if (jumpPressed && player.downTouch) { player.jump(10.0f); }
player.update(dt);
```

Additional resources: [Transform2D](Transform2D.md), [RigidBody3D](RigidBody3D.md).

## Properties

| Property | Description |
| --- | --- |
| [lastPosition](#rigidbody2dlastposition) | Position on the previous frame. |
| [velocity](#rigidbody2dvelocity) | Linear velocity. |
| [acceleration](#rigidbody2dacceleration) | Accumulated acceleration. |
| [maxSpeed](#rigidbody2dmaxspeed) | Speed limit. |
| [drag](#rigidbody2ddrag) | Drag factor. |
| [isEnabled](#rigidbody2disenabled) | Whether the body takes part in physics. |
| [isStatic](#rigidbody2disstatic) | Whether the body is immovable. |
| [useGravity](#rigidbody2dusegravity) | Whether gravity should apply. |
| [bounds](#rigidbody2dbounds) | Rectangle the body is kept inside. |
| [Touch flags](#rigidbody2d-touch-flags) | Which sides are touching something. |

## Public Methods

| Method | Description |
| --- | --- |
| [teleport](#rigidbody2dteleport) | Moves the body straight to a position. |
| [applyGravity](#rigidbody2dapplygravity) | Adds gravity to the acceleration. |
| [applyForce](#rigidbody2dapplyforce) | Adds a force to the acceleration. |
| [updateForce](#rigidbody2dupdateforce) | Updates the body's forces for one frame. |
| [update](#rigidbody2dupdate) | Advances the body by one frame. |
| [getPosition](#rigidbody2dgetposition) | The X and Y of `translation`. |
| [getRectCentered](#rigidbody2dgetrectcentered) | The body as a rectangle, centred version. |
| [getRectCornered](#rigidbody2dgetrectcornered) | The body as a rectangle, cornered version. |
| [jump](#rigidbody2djump) | Replaces vertical velocity with an upward jump. |

## Inherited Members

### Public Methods

| Method | Description |
| --- | --- |
| [getCenter](Transform2D.md#transform2dgetcenter), [getTop … getBottomRight](Transform2D.md#transform2d-edge-points) | Centre, edge and corner points. |
| [getAABB](Transform2D.md#transform2dgetaabb) | The rectangle as a raylib `Rectangle`. |
| [intersectPoint](Transform2D.md#transform2dintersectpoint), [intersectTransform](Transform2D.md#transform2dintersecttransform) | Point and rectangle overlap tests. |

---

## RigidBody2D.lastPosition

### Declaration

```cpp
Vector2 lastPosition = {};
```

### Description

Position on the previous frame. [teleport](#rigidbody2dteleport) sets it together with the position.

---

## RigidBody2D.velocity

### Declaration

```cpp
Vector2 velocity = {};
```

### Description

Linear velocity. Negative Y moves up the screen.

---

## RigidBody2D.acceleration

### Declaration

```cpp
Vector2 acceleration = {};
```

### Description

Accumulated acceleration. [applyGravity](#rigidbody2dapplygravity) and [applyForce](#rigidbody2dapplyforce) add to it.

---

## RigidBody2D.maxSpeed

### Declaration

```cpp
float maxSpeed = 200.0f;
```

### Description

Speed limit.

---

## RigidBody2D.drag

### Declaration

```cpp
float drag = 0.9f;
```

### Description

Drag factor.

---

## RigidBody2D.isEnabled

### Declaration

```cpp
bool isEnabled = true;
```

### Description

Whether the body takes part in physics.

---

## RigidBody2D.isStatic

### Declaration

```cpp
bool isStatic = false;
```

### Description

Whether the body is immovable.

---

## RigidBody2D.useGravity

### Declaration

```cpp
bool useGravity = true;
```

### Description

Whether gravity should apply to the body.

---

## RigidBody2D.bounds

### Declaration

```cpp
Rectangle bounds = {};
```

### Description

Rectangle the body is kept inside, inset by the body's own size so the whole body stays within it. A rectangle with no area, such as the default, means no limit.

---

## RigidBody2D touch flags

### Declaration

```cpp
bool upTouch = false;
bool downTouch = false;
bool leftTouch = false;
bool rightTouch = false;
```

### Description

Which sides of the body are touching something. `downTouch` means the body is standing on something.

---

## RigidBody2D.teleport

### Declaration

```cpp
void teleport(Vector2 newPosition);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `newPosition` | Where to put the body. |

### Description

Moves the body straight to `newPosition`. Sets `translation` to `(newPosition.x, newPosition.y, 0)` and [lastPosition](#rigidbody2dlastposition) to `newPosition`, so the body's previous position matches its new one. Note that this also resets `translation.z` to 0.

---

## RigidBody2D.applyGravity

### Declaration

```cpp
void applyGravity(Vector2 gravity = { 0, 20 });
```

### Parameters

| Parameter | Description |
| --- | --- |
| `gravity` | Gravity to add. The default pulls down the screen. |

### Description

Adds `gravity` to [acceleration](#rigidbody2dacceleration). It does not check [useGravity](#rigidbody2dusegravity); the caller decides.

---

## RigidBody2D.applyForce

### Declaration

```cpp
void applyForce(Vector2 force = { 1, 0 });
```

### Parameters

| Parameter | Description |
| --- | --- |
| `force` | Force to add. Defaults to one unit to the right. |

### Description

Adds `force` to [acceleration](#rigidbody2dacceleration).

---

## RigidBody2D.updateForce

*Declared only.*

### Declaration

```cpp
void updateForce(float deltaTime);
```

### Description

Updates the body's forces for a frame of `deltaTime` seconds.

---

## RigidBody2D.update

*Declared only.*

### Declaration

```cpp
void update(float deltaTime);
```

### Description

Advances the body by a frame of `deltaTime` seconds.

---

## RigidBody2D.getPosition

### Declaration

```cpp
Vector2 getPosition();
```

### Returns

**Vector2** `{ translation.x, translation.y }`.

### Description

The body's position.

---

## RigidBody2D.getRectCentered

*Declared only.*

### Declaration

```cpp
Rectangle getRectCentered();
```

### Description

Returns the body as a rectangle. The name suggests a rectangle positioned by its centre, in contrast to [getRectCornered](#rigidbody2dgetrectcornered).

---

## RigidBody2D.getRectCornered

*Declared only.*

### Declaration

```cpp
Rectangle getRectCornered();
```

### Description

Returns the body as a rectangle. The name suggests a rectangle positioned by its corner, like [getAABB](Transform2D.md#transform2dgetaabb).

---

## RigidBody2D.jump

### Declaration

```cpp
void jump(float force);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `force` | Upward speed of the jump. |

### Description

Replaces the vertical velocity with `-force`, an upward jump in screen coordinates. Any previous falling or rising speed is discarded, so jumps have the same height every time. Check [downTouch](#rigidbody2d-touch-flags) first if the body should only jump from the ground.
