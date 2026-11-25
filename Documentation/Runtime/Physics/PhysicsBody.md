# PhysicsBody

struct in `Physics3D` / Implemented in: [`Physics/PhysicsWorld.h`](../../../Source/Runtime/Physics/PhysicsWorld.h)

> [!WARNING]
> **Not built yet.** `PhysicsWorld.h` includes `Physics.h`, which depends on raylib and `Collider.h`; neither is in this repository.

## Description

One body as the [PhysicsWorld](PhysicsWorld.md) sees it: an id and a pointer to its rigid body.

The world never owns bodies and knows nothing about the objects that own them. Each frame, the game builds a list of `PhysicsBody` entries and passes it to [PhysicsWorld.Solve](PhysicsWorld.md#physicsworldsolve). The world identifies bodies only by [id](#physicsbodyid), so the ids you choose are the ids that come back in each [ContactEvent](ContactEvent.md).

Keep each body's id the same from frame to frame. The world compares this frame's contacts with last frame's by id to work out which started and which ended.

```cpp
std::vector<Physics3D::PhysicsBody> bodies;
for (GameObject& object : objects) {
    bodies.push_back({ object.id, &object.rigidBody3D });
}
std::vector<Physics3D::ContactEvent> events = world.Solve(bodies);
```

Additional resources: [PhysicsWorld](PhysicsWorld.md), [RigidBody3D](RigidBody3D.md), [ContactEvent](ContactEvent.md).

## Properties

| Property | Description |
| --- | --- |
| [id](#physicsbodyid) | The id the body's owner answers to. |
| [rigidBody](#physicsbodyrigidbody) | The body to simulate. |

---

## PhysicsBody.id

### Declaration

```cpp
std::uint64_t id = 0;
```

### Description

The id the body's owner answers to. It must be unique among the bodies passed to one `Solve` call and stay the same across frames.

---

## PhysicsBody.rigidBody

### Declaration

```cpp
RigidBody3D* rigidBody = nullptr;
```

### Description

The body to simulate. It must stay valid for the duration of the [Solve](PhysicsWorld.md#physicsworldsolve) call.
