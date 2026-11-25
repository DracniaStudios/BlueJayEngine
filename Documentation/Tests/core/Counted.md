# Counted

struct in an anonymous namespace / Implemented in: [`Tests/core/MemoryTests.cpp`](../../../Source/Tests/core/MemoryTests.cpp)

> [!NOTE]
> Test fixture. `Counted` exists only inside the memory tests (`bluejay_memory_tests`) and is not part of the engine.

## Description

A struct that counts how many instances are alive, used to test that [PoolAllocator](../../Runtime/Core/PoolAllocator.md) runs constructors and destructors exactly once.

The constructor increments [alive](#countedalive) and the destructor decrements it. The test constructs objects with [PoolAllocator.construct](../../Runtime/Core/PoolAllocator.md#poolallocatorconstruct), checks that `alive` went up and that the constructor arguments arrived, then destroys them and checks that `alive` returns to 0.

```cpp
Counted::alive = 0;
bluejay::memory::PoolAllocator<Counted, 4> pool;

Counted* a = pool.construct(7);   // alive == 1, a->id == 7
pool.destroy(a);                  // alive == 0
```

Additional resources: [PoolAllocator](../../Runtime/Core/PoolAllocator.md), [Vec3](Vec3.md), [OverAligned](OverAligned.md).

## Static Properties

| Property | Description |
| --- | --- |
| [alive](#countedalive) | Number of `Counted` objects currently alive. |

## Properties

| Property | Description |
| --- | --- |
| [id](#countedid) | Value passed to the constructor. |

## Constructors

| Constructor | Description |
| --- | --- |
| [Counted](#countedcounted) | Stores an id and counts the new instance. |

---

## Counted.alive

### Declaration

```cpp
static inline int alive = 0;
```

### Description

Number of `Counted` objects currently alive. The tests reset it to 0 before they start.

---

## Counted.id

### Declaration

```cpp
int id;
```

### Description

Value passed to the constructor. The tests read it back to confirm that `construct` forwarded its arguments.

---

## Counted.Counted

### Declaration

```cpp
explicit Counted(int i);
~Counted();
```

### Parameters

| Parameter | Description |
| --- | --- |
| `i` | Value stored in [id](#countedid). |

### Description

The constructor stores `i` in [id](#countedid) and increments [alive](#countedalive). The destructor decrements `alive`.
