# PhysicsWorld

class in `Physics3D` / Implemented in: [`Physics/PhysicsWorld.h`](../../../Source/Runtime/Physics/PhysicsWorld.h)

> [!WARNING]
> **Not built yet.** `PhysicsWorld.h` includes `Physics.h`, which depends on raylib and `Collider.h`; neither is in this repository. No `.cpp` defines the members marked *declared only*; their descriptions come from the header's comments.

## Description

The collision half of a physics frame, run over every body at once.

Each [RigidBody3D](RigidBody3D.md) integrates in its owner's own update with [RigidBody3D.Update](RigidBody3D.md#rigidbody3dupdate). After all of them, call [Solve](#physicsworldsolve) once with every body. It:

1. Resolves every overlapping pair of active bodies, repeating `iterations` times.
2. Rebuilds every body's [touch flags](RigidBody3D.md#rigidbody3d-touch-flags) against every solid body near it.
3. Returns the pairs that started or stopped touching since the last `Solve`, as [ContactEvent](ContactEvent.md) values.

Every pair follows the same rules, whatever kind of object owns each body:

* A body takes part only while it is enabled and can collide.
* Two static bodies never touch.
* A pair that includes a trigger is reported but never pushed apart.
* A trigger never sets a touch flag.

The world knows bodies only by id, so it needs nothing from the game. It remembers which pairs are touching between calls; call [Reset](#physicsworldreset) when a new world starts reusing ids.

```cpp
#include <Physics/PhysicsWorld.h>

Physics3D::PhysicsWorld world;

void PhysicsFrame(std::vector<GameObject>& objects, float dt) {
    std::vector<Physics3D::PhysicsBody> bodies;
    for (GameObject& object : objects) {
        object.rigidBody3D.Update(dt);
        bodies.push_back({ object.id, &object.rigidBody3D });
    }

    for (const Physics3D::ContactEvent& contact : world.Solve(bodies)) {
        HandleContact(contact);
    }
}

void LoadLevel() {
    world.Reset();   // ids from the old level mean nothing now
}
```

Additional resources: [RigidBody3D](RigidBody3D.md), [PhysicsBody](PhysicsBody.md), [ContactEvent](ContactEvent.md).

## Public Methods

| Method | Description |
| --- | --- |
| [Solve](#physicsworldsolve) | Resolves collisions, rebuilds touch flags and reports contact changes. |
| [IsTouching](#physicsworldistouching) | Whether two bodies were touching after the last solve. |
| [Reset](#physicsworldreset) | Forgets every contact. |

---

## PhysicsWorld.Solve

*Declared only.*

### Declaration

```cpp
std::vector<ContactEvent> Solve(std::span<const PhysicsBody> bodies, int iterations = 8);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `bodies` | Every body to consider this frame. |
| `iterations` | How many times to resolve overlapping pairs. More iterations settle stacks and crowds better but cost more. |

### Returns

**std::vector\<ContactEvent\>** One event for every pair that started or stopped touching since the previous call.

### Description

Resolves collisions, rebuilds touch flags and reports contact changes, in that order. See the [class description](#description) for the rules every pair follows.

A pair that was touching last time but has a body missing from `bodies` this time is reported as an `Exit`.

---

## PhysicsWorld.IsTouching

*Declared only.*

### Declaration

```cpp
bool IsTouching(std::uint64_t a, std::uint64_t b) const;
```

### Parameters

| Parameter | Description |
| --- | --- |
| `a` | Id of one body. |
| `b` | Id of the other body. The order does not matter. |

### Returns

**bool** `true` if the last [Solve](#physicsworldsolve) left these two bodies touching.

### Description

Whether two bodies were touching after the last solve.

---

## PhysicsWorld.Reset

### Declaration

```cpp
void Reset();
```

### Description

Forgets every contact, as for a new world whose ids start over. Call it when you load a new world, so contacts remembered from the old world are not compared against new bodies that happen to reuse the same ids.

---

## Private types

| Type | Description |
| --- | --- |
| `PairKey` | Two body ids, lower first, so `(a, b)` and `(b, a)` are the same key. |
| `PairKeyHash` | Hash for `PairKey`, mixing the two ids with a 64-bit golden-ratio multiplier. |

The world stores the pairs touching after the last solve in an `std::unordered_map<PairKey, bool>`, where the `bool` records whether the pair is a trigger pair. That lets an `Exit` still say whether it was a trigger after one of its bodies is gone.
