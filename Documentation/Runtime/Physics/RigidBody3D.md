# RigidBody3D

struct in `Physics3D` / Inherits from: [Transform3D](Transform3D.md) / Implemented in: [`Physics/Physics.h`](../../../Source/Runtime/Physics/Physics.h)

> [!WARNING]
> **Not built yet.** `Physics.h` uses raylib types and includes `Collider.h` (which defines `Collider3D`, `ColliderVolume` and `ContactInfo`). Neither raylib nor `Collider.h` is in this repository, and no `.cpp` defines the members marked *declared only*. Those descriptions come from the declarations and the comments in the header.

## Description

A body that moves under velocity, acceleration and gravity, and collides as an oriented box.

A `RigidBody3D` is a [Transform3D](Transform3D.md) plus motion state, a collider, lock flags and touch flags. Physics runs in two halves:

1. **Integration.** Each body's owner calls [Update](#rigidbody3dupdate) during its own update.
2. **Collision.** After every body has integrated, [PhysicsWorld.Solve](PhysicsWorld.md#physicsworldsolve) resolves overlaps, rebuilds every body's touch flags and reports contacts.

Collision testing has two phases. [broadPhaseBox](#rigidbody3dbroadphasebox) is a cheap axis-aligned box around the collider, tested with [OverlapsBroadPhase](#rigidbody3doverlapsbroadphase). Only pairs that pass go on to [getContact](#rigidbody3dgetcontact), an exact oriented-box test by the separating axis theorem.

```cpp
#include <Physics/Physics.h>
#include <Physics/PhysicsWorld.h>

Physics3D::RigidBody3D crate;
crate.Teleport(0, 5, 0);
crate.scale = { 1, 1, 1 };

Physics3D::RigidBody3D floor;
floor.isStatic = true;
floor.scale = { 20, 1, 20 };

Physics3D::PhysicsWorld world;
Physics3D::PhysicsBody bodies[] = { { 1, &crate }, { 2, &floor } };

// each frame
crate.Update(dt);
floor.Update(dt);
world.Solve(bodies);
if (crate.downTouch) { crate.Jump(8.0f); }
```

Additional resources: [Transform3D](Transform3D.md), [PhysicsWorld](PhysicsWorld.md), [ContactEvent](ContactEvent.md).

## Properties

| Property | Description |
| --- | --- |
| [isEnabled](#rigidbody3disenabled) | Whether the body takes part in physics. |
| [isStatic](#rigidbody3disstatic) | Whether the body is immovable. |
| [broadPhaseBox](#rigidbody3dbroadphasebox) | Axis-aligned box enclosing the collider, for the broad phase. |
| [collider](#rigidbody3dcollider) | Shape and size of the collision volume. |
| [velocity](#rigidbody3dvelocity) | Linear velocity. |
| [acceleration](#rigidbody3dacceleration) | Linear acceleration. |
| [angularVelocity](#rigidbody3dangularvelocity) | Spin, in radians per second around the vector's axis. |
| [Lock flags](#rigidbody3d-lock-flags) | Freeze parts of the body's motion or transform. |
| [Touch flags](#rigidbody3d-touch-flags) | Which sides were touching a solid body after the last solve. |

## Public Methods

| Method | Description |
| --- | --- |
| [SetGravity](#rigidbody3dsetgravity) | Sets the gravity applied to this body. |
| [SetVelocity](#rigidbody3dsetvelocity) | Sets the linear velocity. |
| [GetVelocity](#rigidbody3dgetvelocity) | Returns the linear velocity. |
| [GetAcceleration](#rigidbody3dgetacceleration) | Returns the linear acceleration. |
| [GetAirTime](#rigidbody3dgetairtime) | Returns how long the body has been airborne. |
| [GetColliderVolume](#rigidbody3dgetcollidervolume) | The collider placed in the world. |
| [GetColliderCenter](#rigidbody3dgetcollidercenter) | World-space centre of the collider. |
| [GetWorldHalfExtents](#rigidbody3dgetworldhalfextents) | World-space half-size of the collider, accounting for rotation. |
| [GetLocalHalfExtents](#rigidbody3dgetlocalhalfextents) | Half-size of the collider along its own axes. |
| [SyncBroadPhaseBox](#rigidbody3dsyncbroadphasebox) | Rebuilds the broad-phase box without stepping the simulation. |
| [ApplyGravity](#rigidbody3dapplygravity) | Applies this body's gravity. |
| [Teleport](#rigidbody3dteleport) | Moves the body straight to a position. |
| [AddForce](#rigidbody3daddforce) | Pushes the body along a direction. |
| [Jump](#rigidbody3djump) | Makes the body jump. |
| [UpdateForce](#rigidbody3dupdateforce) | Updates the body's forces for one frame. |
| [Update](#rigidbody3dupdate) | Integrates the body over one frame. |
| [getContact](#rigidbody3dgetcontact) | Exact contact test against another body. |
| [Raycast](#rigidbody3draycast) | Exact ray test against the collider. |
| [OverlapsBroadPhase](#rigidbody3doverlapsbroadphase) | Cheap conservative overlap test. |
| [isCollidingWith, getCollisionNormal, getPenetrationDepth](#rigidbody3d-contact-shortcuts) | One-value shortcuts over `getContact`. |
| [checkRayCollision](#rigidbody3dcheckraycollision) | Undocumented ray check against another body. |
| [resolveCollision](#rigidbody3dresolvecollision) | Separates this body from an overlapping one. |
| [formatToJson](#rigidbody3dformattojson) | Saves the body to JSON. |
| [loadFromJson](#rigidbody3dloadfromjson) | Loads the body from JSON. |

## Related Functions

| Function | Description |
| --- | --- |
| [DrawOrientedBoxWires](#draworientedboxwires) | Draws a wireframe box that honours rotation. |

## Inherited Members

### Properties

| Property | Description |
| --- | --- |
| `translation`, `rotation`, `scale` | Position, orientation and size, from raylib `Transform`. |
| [forward, back, right, left, up, down](Transform3D.md#transform3d-direction-vectors) | Axis directions. |

### Public Methods

| Method | Description |
| --- | --- |
| [getPosition](Transform3D.md#transform3dgetposition), [getCenter](Transform3D.md#transform3dgetcenter) | Position accessors. |
| [getTop … getBottomRight](Transform3D.md#transform3d-edge-points) | Edge and corner points. |
| [getAABB](Transform3D.md#transform3dgetaabb) | XY footprint rectangle. |

## Private State

These members are private but shape how the public methods behave.

| Member | Default | Description |
| --- | --- | --- |
| `appliedGravity` | `(0, 20, 0)` | Gravity applied to this body. Set with [SetGravity](#rigidbody3dsetgravity). |
| `useGravity` | `true` | Whether gravity applies. |
| `maxSpeed` | `200` | Speed limit. |
| `drag` | `0.9` | Drag factor. |
| `airTime` | `0` | Time airborne. Read with [GetAirTime](#rigidbody3dgetairtime). |
| `lastPosition` | `(0, 0, 0)` | Position on the previous frame. |
| `objectID` | `0` | Identifier. Contact state is kept by [PhysicsWorld](PhysicsWorld.md), by id, not here. |

---

## RigidBody3D.isEnabled

### Declaration

```cpp
bool isEnabled = true;
```

### Description

Whether the body takes part in physics. [PhysicsWorld.Solve](PhysicsWorld.md#physicsworldsolve) only considers bodies that are enabled and can collide.

---

## RigidBody3D.isStatic

### Declaration

```cpp
bool isStatic = false;
```

### Description

Whether the body is immovable, like a floor or wall. Two static bodies never touch each other.

---

## RigidBody3D.broadPhaseBox

### Declaration

```cpp
BoundingBox broadPhaseBox = {};
```

### Description

Axis-aligned box enclosing the collider, used for the broad phase and nothing else.

Because it encloses a possibly rotated or offset collider, it can be much larger than the collider itself. Use it only to rule pairs out with [OverlapsBroadPhase](#rigidbody3doverlapsbroadphase). To ask whether something solid is really at a point, use [Raycast](#rigidbody3draycast) or [getContact](#rigidbody3dgetcontact).

---

## RigidBody3D.collider

### Declaration

```cpp
Collider3D collider = {};
```

### Description

Shape and size of the collision volume. `Collider3D` is defined in `Collider.h`, which is not in this repository. From how it is used, it can be a box or a sphere, can be offset from the body, and can be marked as a trigger.

---

## RigidBody3D.velocity

### Declaration

```cpp
Vector3 velocity = {};
```

### Description

Linear velocity. Also settable with [SetVelocity](#rigidbody3dsetvelocity).

---

## RigidBody3D.acceleration

### Declaration

```cpp
Vector3 acceleration = {};
```

### Description

Linear acceleration.

---

## RigidBody3D.angularVelocity

### Declaration

```cpp
Vector3 angularVelocity = {};
```

### Description

Spin. The vector's direction is the spin axis and its length is the speed, in radians per second.

---

## RigidBody3D lock flags

### Declaration

```cpp
bool lockAngularVelocity = false;
bool lockVelocity = false;
bool lockAcceleration = false;
bool lockTranslation = false;
bool lockRotation = false;
bool lockScale = false;
```

### Description

Each flag freezes one part of the body's motion or transform: angular velocity, velocity, acceleration, position, rotation or scale. All start unlocked.

---

## RigidBody3D touch flags

### Declaration

```cpp
bool upTouch = false;
bool downTouch = false;
bool frontTouch = false;
bool backTouch = false;
bool leftTouch = false;
bool rightTouch = false;
```

### Description

Which sides of the body were touching a solid body after the last [PhysicsWorld.Solve](PhysicsWorld.md#physicsworldsolve), which rebuilds them every time. Triggers never set them.

| Flag | Meaning (from the source comments) |
| --- | --- |
| `upTouch` | Touching something above. |
| `downTouch` | Touching something below: the body is grounded. |
| `frontTouch` | Touching on the +X side. |
| `backTouch` | Touching on the -X side. |
| `leftTouch` | Touching on the -Z side. |
| `rightTouch` | Touching on the +Z side. |

---

## RigidBody3D.SetGravity

### Declaration

```cpp
void SetGravity(Vector3 gravity);
void SetGravity(float x, float y, float z);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `gravity` | Gravity acceleration for this body. |
| `x`, `y`, `z` | The same, as separate components. |

### Description

Sets the gravity applied to this body. The default is `(0, 20, 0)`.

---

## RigidBody3D.SetVelocity

### Declaration

```cpp
void SetVelocity(Vector3 velocity);
void SetVelocity(float x, float y, float z);
```

### Description

Sets [velocity](#rigidbody3dvelocity).

---

## RigidBody3D.GetVelocity

### Declaration

```cpp
Vector3 GetVelocity() const;
```

### Returns

**Vector3** The current [velocity](#rigidbody3dvelocity).

### Description

Returns the linear velocity.

---

## RigidBody3D.GetAcceleration

### Declaration

```cpp
Vector3 GetAcceleration() const;
```

### Returns

**Vector3** The current [acceleration](#rigidbody3dacceleration).

### Description

Returns the linear acceleration.

---

## RigidBody3D.GetAirTime

### Declaration

```cpp
float GetAirTime() const;
```

### Returns

**float** How long the body has been airborne.

### Description

Returns how long the body has been airborne. The value is maintained by members that are declared only, so how it is counted is not visible in this repository.

---

## RigidBody3D.GetColliderVolume

### Declaration

```cpp
ColliderVolume GetColliderVolume() const;
```

### Returns

**ColliderVolume** The collider placed in the world: its shape, centre, axes and half-extents.

### Description

Builds the world-space collision volume from [collider](#rigidbody3dcollider) and the body's `translation`, `rotation` and `scale`. The other collider queries on this page are built on it.

---

## RigidBody3D.GetColliderCenter

### Declaration

```cpp
Vector3 GetColliderCenter() const;
```

### Returns

**Vector3** World-space centre of the collision volume.

### Description

World-space centre of the collider. It differs from `translation` when the collider is offset.

---

## RigidBody3D.GetWorldHalfExtents

### Declaration

```cpp
Vector3 GetWorldHalfExtents() const;
```

### Returns

**Vector3** Half-size of the collider on the world X, Y and Z axes.

### Description

World-space half-size of the collider, accounting for rotation: half the size of the smallest axis-aligned box around the rotated collider.

---

## RigidBody3D.GetLocalHalfExtents

### Declaration

```cpp
Vector3 GetLocalHalfExtents() const;
```

### Returns

**Vector3** Half-size of the collider along its own axes. By default this is `scale * 0.5f`.

### Description

Half-size of the collider along its own axes, before rotation.

---

## RigidBody3D.SyncBroadPhaseBox

*Declared only.*

### Declaration

```cpp
void SyncBroadPhaseBox();
```

### Description

Rebuilds [broadPhaseBox](#rigidbody3dbroadphasebox) from the current transform without stepping the simulation, repairing a degenerate scale or rotation on the way. For editor tools that move a body while the simulation is paused.

---

## RigidBody3D.ApplyGravity

*Declared only.*

### Declaration

```cpp
void ApplyGravity();
```

### Description

Applies this body's gravity, as set by [SetGravity](#rigidbody3dsetgravity).

---

## RigidBody3D.Teleport

*Declared only.*

### Declaration

```cpp
void Teleport(Vector3 newPosition);
void Teleport(float x, float y, float z);
```

### Description

Moves the body straight to a new position, rather than moving it there through the simulation.

---

## RigidBody3D.AddForce

*Declared only.*

### Declaration

```cpp
void AddForce(Vector3 forceDirection, float force);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `forceDirection` | Direction to push in. |
| `force` | Strength of the push. |

### Description

Pushes the body along `forceDirection` with strength `force`.

---

## RigidBody3D.Jump

*Declared only.*

### Declaration

```cpp
void Jump(float force);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `force` | Strength of the jump. |

### Description

Makes the body jump. Check [downTouch](#rigidbody3d-touch-flags) first if the body should only jump from the ground.

---

## RigidBody3D.UpdateForce

*Declared only.*

### Declaration

```cpp
void UpdateForce(float deltaTime);
```

### Description

Updates the body's forces for a frame of `deltaTime` seconds.

---

## RigidBody3D.Update

*Declared only.*

### Declaration

```cpp
void Update(float deltaTime);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `deltaTime` | Seconds since the previous frame. |

### Description

Integrates the body over one frame. Collisions and touch flags are handled afterwards by [PhysicsWorld.Solve](PhysicsWorld.md#physicsworldsolve), over every body at once.

Comments elsewhere in the source say that `Update` repairs a scale of exactly zero and a zeroed rotation quaternion. It does not repair negative scale.

---

## RigidBody3D.getContact

*Declared only.*

### Declaration

```cpp
ContactInfo getContact(const RigidBody3D& other) const;
```

### Parameters

| Parameter | Description |
| --- | --- |
| `other` | The body to test against. |

### Returns

**ContactInfo** Whether the two colliders overlap and, if so, the contact details. `ContactInfo` is defined in `Collider.h`.

### Description

The narrow phase: an exact oriented-box overlap test by the separating axis theorem (15 axes). Run [OverlapsBroadPhase](#rigidbody3doverlapsbroadphase) first to skip pairs that cannot touch.

---

## RigidBody3D.Raycast

### Declaration

```cpp
bool Raycast(Ray ray, float& outDistance, Vector3& outNormal) const;
```

### Parameters

| Parameter | Description |
| --- | --- |
| `ray` | The ray to test. |
| `outDistance` | Receives the distance along the ray to the hit. |
| `outNormal` | Receives the surface normal at the hit. |

### Returns

**bool** `true` if the ray hits the collider.

### Description

Exact ray test against the collider, unlike testing [broadPhaseBox](#rigidbody3dbroadphasebox). Use it whenever you need to know whether something solid is really there: the broad-phase box reports hits in the empty space around a rotated ramp, an offset collider or a sphere. When the ray starts inside the collider, the exit distance is returned.

---

## RigidBody3D.OverlapsBroadPhase

### Declaration

```cpp
bool OverlapsBroadPhase(const RigidBody3D& other) const;
```

### Parameters

| Parameter | Description |
| --- | --- |
| `other` | The body to test against. |

### Returns

**bool** `true` if the two broad-phase boxes overlap.

### Description

Cheap conservative overlap test between the two bodies' broad-phase boxes. `false` means the colliders cannot be touching and [getContact](#rigidbody3dgetcontact) can be skipped. `true` only means the pair is worth testing properly.

---

## RigidBody3D contact shortcuts

*Declared only.*

### Declaration

```cpp
bool isCollidingWith(const RigidBody3D& other) const;
Vector3 getCollisionNormal(const RigidBody3D& other) const;
float getPenetrationDepth(const RigidBody3D& other) const;
```

### Description

Each returns one value from [getContact](#rigidbody3dgetcontact): whether the bodies overlap, the contact normal, or how deep they overlap. Every call runs a full separating-axis test, so when you need more than one of these values, call `getContact` once instead.

---

## RigidBody3D.checkRayCollision

*Declared only.*

### Declaration

```cpp
void checkRayCollision(const RigidBody3D& other);
```

### Description

A ray-based check against another body. The header gives no description and there is no definition in this repository, so its behaviour is not documented.

---

## RigidBody3D.resolveCollision

*Declared only.*

### Declaration

```cpp
void resolveCollision(RigidBody3D& other);
void resolveCollision(RigidBody3D& other, const ContactInfo& contact);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `other` | The overlapping body. |
| `contact` | A contact already computed by [getContact](#rigidbody3dgetcontact). |

### Description

Separates this body from an overlapping one. The second overload takes a contact the caller already computed, so the solver does not repeat the narrow-phase test.

---

## RigidBody3D.formatToJson

*Declared only.*

### Declaration

```cpp
Json formatToJson();
```

### Returns

**Json** (`nlohmann::json`) The body's saved state.

### Description

Saves the body to JSON.

---

## RigidBody3D.loadFromJson

*Declared only.*

### Declaration

```cpp
bool loadFromJson(const Json& j);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `j` | JSON produced by [formatToJson](#rigidbody3dformattojson). |

### Returns

**bool** Whether the load succeeded.

### Description

Loads the body from JSON.

---

## DrawOrientedBoxWires

*Declared only.*

### Declaration

```cpp
void DrawOrientedBoxWires(Vector3 center, Vector3 size, Quaternion rotation, Color color);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `center` | Centre of the box. |
| `size` | Full size of the box on each of its own axes. |
| `rotation` | Orientation of the box. |
| `color` | Line colour. |

### Description

Draws a wireframe box that honours rotation, unlike raylib's axis-aligned `DrawCubeWires` and `DrawBoundingBox`. The collider debug view and the World Editor's selection outline both use it, so they match what is actually drawn.
