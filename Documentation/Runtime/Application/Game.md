# Game

abstract class in global namespace / Implemented in: [`Application/Application.h`](../../../Source/Runtime/Application/Application.h)

## Description

Base class for a game that the engine's main loop drives.

Derive from `Game`, implement [Update](#gameupdate) and [Render](#gamerender), and pass an instance to [Run](#run). `Run` owns the window, the input system, the [Renderer](../Rendering/Renderer.md) and the loop. Your game only decides what happens each frame and what to draw.

Each frame, `Run` does the following, in this order:

1. Handles window and input events. A quit request ends the loop, and every event goes to [BindingHotkeys](../InputSystem/BindingHotkeys.md), which forwards it to the [InputSystem](../InputSystem/InputSystem.md).
2. Measures the frame time with a [Clock](../Core/Clock.md).
3. Calls [InputSystem.Update](../InputSystem/InputSystem.md#inputsystemupdate).
4. Calls your `Update(dt)`. Returning `false` ends the loop after this frame.
5. Calls your `Render(renderer)`.
6. Shows the renderer's pixels in the window.

```cpp
#include <Application/Application.h>
#include <InputSystem/InputSystem.h>
#include <Rendering/Camera.h>
#include <Rendering/Window.h>

class MyGame : public Game {
public:
    bool Update(float dt) override {
        auto& input = InputSystem::InputSystem::getInstance();
        angle_ += dt;
        return !input.IsActionPressed(InputSystem::ACTION_UI_PAUSE);   // Esc quits
    }

    void Render(Renderer& renderer) override {
        renderer.Clear(0xFF101418);
        renderer.DrawMesh(cube_, RotationY(angle_), camera_.View(), proj_,
                          camera_.ToViewSpace(Normalize({ 0.4f, 0.8f, 0.6f })));
    }

private:
    const Mesh cube_ = MakeCube();
    const Matrix4x4 proj_ = Perspective(1.05f, float(SCREEN_WIDTH) / SCREEN_HEIGHT, 0.1f, 100.0f);
    Camera camera_;
    float angle_ = 0.0f;
};

int main() {
    MyGame game;
    return Run(AppConfig{}, game);
}
```

Additional resources: [AppConfig](AppConfig.md), [Renderer](../Rendering/Renderer.md), [InputSystem](../InputSystem/InputSystem.md).

## Public Methods

| Method | Description |
| --- | --- |
| [Update](#gameupdate) | Advances the game by one frame. Return `false` to quit. |
| [Render](#gamerender) | Draws the current frame. |

## Related Functions

| Function | Description |
| --- | --- |
| [Run](#run) | Creates the window and runs the main loop until the game quits. |

---

## Game.Update

### Declaration

```cpp
virtual bool Update(float dt) = 0;
```

### Parameters

| Parameter | Description |
| --- | --- |
| `dt` | Seconds since the previous frame. |

### Returns

**bool** `true` to keep running, `false` to quit.

### Description

Advances the game by one frame.

Called once per frame, after input has been updated, so [InputSystem.IsActionPressed](../InputSystem/InputSystem.md#inputsystemisactionpressed) and the other input queries already describe this frame. Scale movement by `dt` so it does not depend on the frame rate.

When it returns `false`, `Run` still calls [Render](#gamerender) once for this frame and then exits.

---

## Game.Render

### Declaration

```cpp
virtual void Render(Renderer& renderer) = 0;
```

### Parameters

| Parameter | Description |
| --- | --- |
| `renderer` | The software renderer whose pixels are shown after this call. |

### Description

Draws the current frame.

`Run` does not clear the frame for you. Start every `Render` with [Renderer.Clear](../Rendering/Renderer.md#rendererclear): it resets the colour and depth buffers, and it also frees the per-frame scratch memory that [Renderer.DrawMesh](../Rendering/Renderer.md#rendererdrawmesh) allocates from. Skipping it eventually makes `DrawMesh` throw `std::bad_alloc`.

---

## Run

### Declaration

```cpp
int Run(const AppConfig& config, Game& game);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `config` | Window title, bindings file, rebind hotkeys and mouse capture. See [AppConfig](AppConfig.md). |
| `game` | The game to drive. |

### Returns

**int** The process exit code: `0` after a normal quit, `1` if the window, renderer or framebuffer texture could not be created.

### Description

Creates the window and runs the main loop until the game quits.

`Run` sets up the following before the first frame:

* A [Window](../Rendering/Window.md) titled `config.title`, with mouse capture set from `config.captureMouse`.
* The [InputSystem](../InputSystem/InputSystem.md), with bindings loaded from `config.bindingsFile`.
* [BindingHotkeys](../InputSystem/BindingHotkeys.md) for the actions in `config.rebindable`.
* A [Renderer](../Rendering/Renderer.md) of `SCREEN_WIDTH` x `SCREEN_HEIGHT` (1280 x 720) pixels.

The loop ends when the window is closed or [Update](#gameupdate) returns `false`. `Run` then shuts the input system down and destroys the window, which shuts SDL down. Return its result from `main`.
