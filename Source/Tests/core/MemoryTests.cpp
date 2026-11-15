#include <bluejay/core/ArenaAllocator.h>
#include <bluejay/core/PoolAllocator.h>

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <new>

namespace {

int g_failures = 0;

void Check(bool ok, const char* what) {
    if (!ok) {
        std::fprintf(stderr, "FAIL: %s\n", what);
        ++g_failures;
    }
}

bool IsAligned(const void* p, std::size_t alignment) {
    return reinterpret_cast<std::uintptr_t>(p) % alignment == 0;
}

// 12 bytes, 4-aligned: the shape PoolAllocator's slot stride was fixed for.
// A 12-byte stride on a 64-bit target let the free-list pointers overlap slots.
struct Vec3 {
    float x, y, z;
};

struct alignas(32) OverAligned {
    float v[8];
};

struct Counted {
    static inline int alive = 0;
    int id;
    explicit Counted(int i) : id(i) { ++alive; }
    ~Counted() { --alive; }
};

template <typename F>
bool Throws(F&& f) {
    try {
        f();
    } catch (const std::bad_alloc&) {
        return true;
    }
    return false;
}

void ArenaTests() {
    using bluejay::memory::Arena;

    {   // Alignment is honoured for every power-of-two request, including > 16.
        Arena arena(4096);
        const std::size_t alignments[] = {1, 2, 4, 8, 16, 32, 64};
        for (std::size_t alignment : alignments) {
            void* p = arena.allocate(1, alignment);
            Check(IsAligned(p, alignment), "arena result is aligned as requested");
        }
    }

    {   // Blocks do not overlap and do not corrupt each other.
        Arena arena(256);
        auto* a = static_cast<unsigned char*>(arena.allocate(16, 1));
        auto* b = static_cast<unsigned char*>(arena.allocate(16, 1));
        Check(b >= a + 16, "second block starts after the first ends");
        std::memset(a, 0xAA, 16);
        std::memset(b, 0xBB, 16);
        bool intact = true;
        for (int i = 0; i < 16; ++i) { intact = intact && a[i] == 0xAA && b[i] == 0xBB; }
        Check(intact, "writes to one block do not touch the other");
    }

    {   // reset() hands out the same memory again from the start.
        Arena arena(128);
        void* first = arena.allocate(32, 8);
        arena.allocate(32, 8);
        arena.reset();
        Check(arena.allocate(32, 8) == first, "reset restarts allocation at the beginning");
    }

    {   // Running out throws instead of returning memory it does not have.
        Arena arena(64);
        arena.allocate(64, 1);
        Check(Throws([&] { arena.allocate(1, 1); }), "full arena throws bad_alloc");
        arena.reset();
        Check(!Throws([&] { arena.allocate(64, 1); }), "arena is usable again after reset");
        Check(Throws([&] { arena.reset(); arena.allocate(65, 1); }), "request larger than the arena throws");
    }
}

void PoolTests() {
    using bluejay::memory::PoolAllocator;

    {   // Regression: every slot holds its own value with no overlap.
        constexpr std::size_t kCount = 8;
        PoolAllocator<Vec3, kCount> pool;
        Vec3* slots[kCount];
        for (std::size_t i = 0; i < kCount; ++i) {
            const float f = static_cast<float>(i);
            slots[i] = pool.construct(Vec3{f, f + 0.25f, f + 0.5f});
        }
        bool intact = true;
        for (std::size_t i = 0; i < kCount; ++i) {
            const float f = static_cast<float>(i);
            intact = intact && slots[i]->x == f && slots[i]->y == f + 0.25f && slots[i]->z == f + 0.5f;
            intact = intact && IsAligned(slots[i], alignof(Vec3));
        }
        Check(intact, "pool slots do not overlap and are aligned");
        for (std::size_t i = 0; i < kCount; ++i) { pool.destroy(slots[i]); }
    }

    {   // Full pool throws; a destroyed slot is reused (last freed, first reused).
        PoolAllocator<Vec3, 2> pool;
        Vec3* a = pool.construct(Vec3{1, 1, 1});
        Vec3* b = pool.construct(Vec3{2, 2, 2});
        Check(Throws([&] { pool.construct(Vec3{3, 3, 3}); }), "full pool throws bad_alloc");
        pool.destroy(a);
        Check(pool.construct(Vec3{4, 4, 4}) == a, "freed slot is reused");
        pool.destroy(b);
    }

    {   // Constructors and destructors run exactly once per object.
        Counted::alive = 0;
        PoolAllocator<Counted, 4> pool;
        Counted* a = pool.construct(7);
        Counted* b = pool.construct(9);
        Check(Counted::alive == 2 && a->id == 7 && b->id == 9, "construct runs constructors with arguments");
        pool.destroy(a);
        Check(Counted::alive == 1, "destroy runs the destructor");
        pool.destroy(b);
        Check(Counted::alive == 0, "all destructors ran");
    }

    {   // Over-aligned types get over-aligned slots.
        PoolAllocator<OverAligned, 4> pool;
        OverAligned* a = pool.construct();
        OverAligned* b = pool.construct();
        Check(IsAligned(a, 32) && IsAligned(b, 32), "over-aligned slots are aligned");
        pool.destroy(a);
        pool.destroy(b);
    }

    {   // destroy(nullptr) is a harmless no-op.
        PoolAllocator<Vec3, 1> pool;
        pool.destroy(nullptr);
        Check(!Throws([&] { pool.construct(Vec3{0, 0, 0}); }), "pool still works after destroy(nullptr)");
    }
}

}  // namespace

int main() {
    ArenaTests();
    PoolTests();
    return g_failures == 0 ? 0 : 1;
}
