# ContactEvent

struct in `Physics3D` / Implemented in: [`Physics/PhysicsWorld.h`](../../../Source/Runtime/Physics/PhysicsWorld.h)

> [!WARNING]
> **Not built yet.** `PhysicsWorld.h` includes `Physics.h`, which depends on raylib and `Collider.h`; neither is in this repository.

## Description

Reports that two bodies started or stopped touching.

[PhysicsWorld.Solve](PhysicsWorld.md#physicsworldsolve) returns one `ContactEvent` per pair whose contact changed since the previous solve. Each event describes a **pair**, not one body: [a](#contacteventa) and [b](#contacteventb) are the two ids with the lower one first, and both bodies are equally part of the contact. A rule that cares about one kind of body should check both sides.

The world only reports contacts. Deciding what a contact means, such as playing a sound or dealing damage, is up to the code that reads the events.

```cpp
for (const Physics3D::ContactEvent& contact : world.Solve(bodies)) {
    if (contact.phase != Physics3D::ContactPhase::Enter) { continue; }

    if (contact.isTrigger) {
        OnTriggerEntered(contact.a, contact.b);
    } else if (contact.closingSpeed > 5.0f) {
        PlayImpactSound(contact.faster, contact.closingSpeed);
    }
}
```

Additional resources: [PhysicsWorld](PhysicsWorld.md), [PhysicsBody](PhysicsBody.md).

## Properties

| Property | Description |
| --- | --- |
| [a](#contacteventa) | Lower id of the pair. |
| [b](#contacteventb) | Higher id of the pair. |
| [phase](#contacteventphase) | Whether the contact started or ended. |
| [isTrigger](#contacteventistrigger) | Whether either body is a trigger. |
| [normal](#contacteventnormal) | Contact direction, from `a` toward `b`. Enter only. |
| [closingSpeed](#contacteventclosingspeed) | How fast the bodies were approaching. Enter only. |
| [faster](#contacteventfaster) | Id of the faster-moving body. Enter only. |

## Related Types

| Type | Description |
| --- | --- |
| [ContactPhase](#contactphase) | Whether a contact started or ended. |

---

## ContactEvent.a

### Declaration

```cpp
std::uint64_t a = 0;
```

### Description

The lower of the pair's two [PhysicsBody.id](PhysicsBody.md#physicsbodyid) values.

---

## ContactEvent.b

### Declaration

```cpp
std::uint64_t b = 0;
```

### Description

The higher of the pair's two ids.

---

## ContactEvent.phase

### Declaration

```cpp
ContactPhase phase = ContactPhase::Enter;
```

### Description

`Enter` when the pair started touching in this solve, `Exit` when it stopped. An `Exit` is also reported when one of the bodies is no longer passed to `Solve`.

---

## ContactEvent.isTrigger

### Declaration

```cpp
bool isTrigger = false;
```

### Description

`true` when either collider is a trigger. A trigger pair is reported but never pushed apart, and it never sets touch flags. An `Exit` keeps the value from when the contact started, even if one body is gone.

---

## ContactEvent.normal

### Declaration

```cpp
Vector3 normal = {};
```

### Description

Unit vector pointing from `a` toward `b`, measured at first touch before the overlap was resolved. Only set for `Enter`.

---

## ContactEvent.closingSpeed

### Declaration

```cpp
float closingSpeed = 0.0f;
```

### Description

How fast `a` and `b` were approaching each other along [normal](#contacteventnormal) at first touch. Use it to scale impact effects. Only set for `Enter`.

---

## ContactEvent.faster

### Declaration

```cpp
std::uint64_t faster = 0;
```

### Description

Whichever of `a` and `b` was moving faster at first touch, or `a` on a tie. Useful for deciding which body caused an impact. Only set for `Enter`.

---

## ContactPhase

### Declaration

```cpp
enum class ContactPhase { Enter, Exit };
```

### Description

| Value | Description |
| --- | --- |
| `Enter` | The pair started touching. |
| `Exit` | The pair stopped touching. |
