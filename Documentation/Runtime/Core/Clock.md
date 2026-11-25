# Clock

class in global namespace / Implemented in: [`Core/Clock.h`](../../../Source/Runtime/Core/Clock.h)

## Description

Measures the time between frames.

A `Clock` remembers the last moment [Tick](#clocktick) was called. Each call returns the seconds that passed since the previous call and starts a new interval. The first call measures from the moment the clock was constructed.

The clock uses `std::chrono::steady_clock`, so it is never affected by changes to the system time.

[Run](../Application/Game.md#run) creates one `Clock` and passes the value of `Tick()` to [Game.Update](../Application/Game.md#gameupdate) as `dt` every frame.

```cpp
#include <Core/Clock.h>

Clock clock;
while (running) {
    const float dt = clock.Tick();   // seconds since the last frame
    position = position + velocity * dt;
}
```

Additional resources: [Game](../Application/Game.md).

## Public Methods

| Method | Description |
| --- | --- |
| [Tick](#clocktick) | Returns the seconds since the previous call and starts a new interval. |

---

## Clock.Tick

### Declaration

```cpp
float Tick();
```

### Returns

**float** Seconds since the previous call to `Tick`, or since construction on the first call.

### Description

Returns the seconds since the previous call and starts a new interval.

Call it once per frame and use the result as the frame's delta time.
