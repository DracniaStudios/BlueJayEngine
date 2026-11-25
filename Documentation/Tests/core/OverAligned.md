# OverAligned

struct in an anonymous namespace / Implemented in: [`Tests/core/MemoryTests.cpp`](../../../Source/Tests/core/MemoryTests.cpp)

> [!NOTE]
> Test fixture. `OverAligned` exists only inside the memory tests (`bluejay_memory_tests`) and is not part of the engine.

## Description

A 32-byte struct that requires 32-byte alignment, used to test that [PoolAllocator](../../Runtime/Core/PoolAllocator.md) honours alignment stricter than the default.

Ordinary `new` only guarantees `alignof(std::max_align_t)`, typically 16 bytes. The test builds a pool of `OverAligned` and checks that every object it returns sits on a 32-byte boundary, which proves the pool allocates its buffer with the alignment of its slots.

```cpp
bluejay::memory::PoolAllocator<OverAligned, 4> pool;
OverAligned* a = pool.construct();
// reinterpret_cast<std::uintptr_t>(a) % 32 == 0
pool.destroy(a);
```

Additional resources: [PoolAllocator](../../Runtime/Core/PoolAllocator.md), [Vec3](Vec3.md), [Counted](Counted.md).

## Properties

| Property | Description |
| --- | --- |
| [v](#overalignedv) | Eight floats of payload. |

---

## OverAligned.v

### Declaration

```cpp
struct alignas(32) OverAligned { float v[8]; };
```

### Description

Eight floats of payload, which make the struct exactly 32 bytes. The tests never read them; only the struct's address matters.
