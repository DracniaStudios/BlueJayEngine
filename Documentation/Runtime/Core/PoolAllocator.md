# PoolAllocator

class template in `bluejay::memory` / Implemented in: [`Core/PoolAllocator.h`](../../../Source/Runtime/Core/PoolAllocator.h)

## Description

A fixed-capacity allocator for objects of one type.

`PoolAllocator<Object, ObjectCount>` reserves room for exactly `ObjectCount` objects of type `Object` in one contiguous, correctly aligned buffer. [construct](#poolallocatorconstruct) builds an object in a free slot, and [destroy](#poolallocatordestroy) runs its destructor and returns the slot to the pool. Free slots are kept in a linked list stored inside the empty slots themselves, so both operations run in constant time and the pool needs no extra memory.

Freed slots are reused last-in, first-out: the most recently destroyed slot is the next one handed out.

Use a pool when many objects of the same type are created and destroyed in any order, such as particles, projectiles or pooled game objects.

| Template parameter | Description |
| --- | --- |
| `Object` | Type stored in the pool. Over-aligned types (for example `alignas(32)`) are supported. |
| `ObjectCount` | Number of slots. Must be greater than zero (checked by a `static_assert`). |

The pool's destructor frees the buffer but does **not** destroy objects that are still alive. Call [destroy](#poolallocatordestroy) for every object before the pool goes out of scope.

`PoolAllocator` cannot be copied or moved.

```cpp
#include <Core/PoolAllocator.h>

struct Bullet {
    float x, y, speed;
    Bullet(float x, float y, float speed) : x(x), y(y), speed(speed) {}
};

bluejay::memory::PoolAllocator<Bullet, 256> bullets;

Bullet* Fire(float x, float y) {
    return bullets.construct(x, y, 30.0f);   // throws std::bad_alloc when all 256 are in use
}

void Expire(Bullet* bullet) {
    bullets.destroy(bullet);                 // slot goes back to the pool
}
```

Additional resources: [Arena](Arena.md).

## Constructors

| Constructor | Description |
| --- | --- |
| [PoolAllocator](#poolallocatorpoolallocator) | Allocates storage for `ObjectCount` objects and marks every slot free. |

## Public Methods

| Method | Description |
| --- | --- |
| [construct](#poolallocatorconstruct) | Builds an object in a free slot and returns it. |
| [destroy](#poolallocatordestroy) | Destroys an object and returns its slot to the pool. |

---

## PoolAllocator.PoolAllocator

### Declaration

```cpp
PoolAllocator();
```

### Description

Allocates storage for `ObjectCount` objects and marks every slot free.

Each slot is the size of an internal `Node` union that holds either an `Object` or a free-list pointer, padded to the stricter of their two alignments. The buffer is allocated with that alignment, so every slot is correctly aligned for `Object`, including over-aligned types.

---

## PoolAllocator.construct

### Declaration

```cpp
template <typename... Args>
Object* construct(Args&&... args);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `args` | Arguments forwarded to `Object`'s constructor. |

### Returns

**Object\*** The newly constructed object.

### Description

Builds an object in a free slot and returns it.

Takes the first slot from the free list and constructs `Object(std::forward<Args>(args)...)` in it with placement new.

Throws `std::bad_alloc` when every slot is in use.

---

## PoolAllocator.destroy

### Declaration

```cpp
void destroy(Object* ptr) noexcept;
```

### Parameters

| Parameter | Description |
| --- | --- |
| `ptr` | An object returned by this pool's [construct](#poolallocatorconstruct), or `nullptr`. |

### Description

Destroys an object and returns its slot to the pool.

Runs `ptr->~Object()` and pushes the slot onto the front of the free list, so it is the next slot [construct](#poolallocatorconstruct) hands out. Passing `nullptr` does nothing.

Only pass pointers that came from this pool, and destroy each object once.

---

## Private types

| Type | Description |
| --- | --- |
| `Node` | Union of `Node* next` and `alignas(Object) char storage[sizeof(Object)]`. One slot of the pool: holds the object while it is alive and the free-list link while it is free. |
