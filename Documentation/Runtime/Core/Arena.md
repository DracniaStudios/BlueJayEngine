# Arena

class in `bluejay::memory` / Implemented in: [`Core/ArenaAllocator.h`](../../../Source/Runtime/Core/ArenaAllocator.h)

## Description

A linear ("bump") allocator that hands out memory from one fixed block.

An `Arena` allocates a single buffer when it is constructed. Each call to [allocate](#arenaallocate) returns the next correctly aligned piece of that buffer and moves an offset forward. Memory is never freed piece by piece: [reset](#arenareset) rewinds the offset to the start and makes the whole block available again in constant time.

Use an arena for short-lived data that is created and discarded together, such as per-frame scratch buffers. The [Renderer](../Rendering/Renderer.md) uses a 1 MiB arena for its clip-space vertex buffers and resets it every time the frame is cleared.

The arena works with raw bytes. It does not call constructors or destructors, so store trivially destructible types in it, or run destructors yourself before you call `reset`.

`Arena` cannot be copied or moved.

```cpp
#include <Core/ArenaAllocator.h>

struct Particle { float x, y, z, life; };

bluejay::memory::Arena frameArena(64 * 1024);   // 64 KiB

void Frame(size_t count) {
    frameArena.reset();   // everything from last frame is gone

    auto* particles = static_cast<Particle*>(
        frameArena.allocate(count * sizeof(Particle), alignof(Particle)));

    for (size_t i = 0; i < count; ++i) {
        particles[i] = { 0, 0, 0, 1.0f };
    }
}
```

Additional resources: [PoolAllocator](PoolAllocator.md), [Renderer](../Rendering/Renderer.md).

## Constructors

| Constructor | Description |
| --- | --- |
| [Arena](#arenaarena) | Allocates a block of the given size for the arena to hand out. |

## Public Methods

| Method | Description |
| --- | --- |
| [allocate](#arenaallocate) | Returns the next aligned piece of the block. |
| [reset](#arenareset) | Makes the whole block available again. |

---

## Arena.Arena

### Declaration

```cpp
explicit Arena(size_t bytes);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `bytes` | Size of the block, in bytes. |

### Description

Allocates a block of `bytes` bytes for the arena to hand out.

The block is allocated once, here, and freed when the arena is destroyed. Copying is deleted, so two arenas never share a block.

---

## Arena.allocate

### Declaration

```cpp
void* allocate(size_t bytes, size_t alignment = alignof(std::max_align_t));
```

### Parameters

| Parameter | Description |
| --- | --- |
| `bytes` | Number of bytes to allocate. |
| `alignment` | Alignment of the returned address, in bytes. Must be a power of two. Defaults to `alignof(std::max_align_t)`. |

### Returns

**void\*** A pointer to `bytes` bytes of uninitialised memory, aligned to `alignment`.

### Description

Returns the next aligned piece of the block.

Padding is added before the allocation where it is needed to meet `alignment`, and the arena's offset moves to the end of the new allocation. Allocations never overlap and stay valid until the next [reset](#arenareset) or until the arena is destroyed.

Throws `std::bad_alloc` when the rest of the block cannot hold `bytes` bytes at the requested alignment. Nothing is allocated in that case.

```cpp
bluejay::memory::Arena arena(256);
void* a = arena.allocate(16, 16);   // 16-byte aligned
void* b = arena.allocate(32);       // max_align_t aligned, starts after a
```

---

## Arena.reset

### Declaration

```cpp
void reset() noexcept;
```

### Description

Makes the whole block available again.

Rewinds the offset to the start of the block. The next [allocate](#arenaallocate) returns the same address as the first allocation did. Every pointer the arena returned before the reset is invalid afterwards. No destructors run.
