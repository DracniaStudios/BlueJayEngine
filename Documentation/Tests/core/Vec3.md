# Vec3

struct in an anonymous namespace / Implemented in: [`Tests/core/MemoryTests.cpp`](../../../Source/Tests/core/MemoryTests.cpp)

> [!NOTE]
> Test fixture. `Vec3` exists only inside the memory tests (`bluejay_memory_tests`) and is not part of the engine. For the engine's vector type, see [Vector3](../../Runtime/Core/Vector3.md).

## Description

A 12-byte, 4-byte-aligned struct of three floats, used to test [PoolAllocator](../../Runtime/Core/PoolAllocator.md).

Its shape is deliberate. On a 64-bit target a `Vec3` is smaller than the pool's free-list pointer (8 bytes, 8-aligned) and not a multiple of its alignment. An earlier version of the pool sized its slots as 12 bytes for this type, so the free-list pointers overlapped neighbouring slots and the last one was written past the end of the buffer. The tests fill a pool of `Vec3` and check that every slot keeps its own value and is correctly aligned, to make sure that bug stays fixed.

```cpp
bluejay::memory::PoolAllocator<Vec3, 8> pool;
Vec3* v = pool.construct(Vec3{ 1.0f, 1.25f, 1.5f });
pool.destroy(v);
```

Additional resources: [PoolAllocator](../../Runtime/Core/PoolAllocator.md), [OverAligned](OverAligned.md), [Counted](Counted.md).

## Properties

| Property | Description |
| --- | --- |
| [x, y, z](#vec3-properties) | The three components. |

---

## Vec3 properties

### Declaration

```cpp
struct Vec3 { float x, y, z; };
```

### Description

The three components. They have no default values; the tests always initialise them.
