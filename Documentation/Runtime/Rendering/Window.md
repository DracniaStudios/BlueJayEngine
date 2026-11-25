# Window

class in global namespace / Implemented in: [`Rendering/Window.h`](../../../Source/Runtime/Rendering/Window.h)

## Description

An SDL3 window that shows a CPU-drawn framebuffer.

`Window` starts SDL, creates a resizable window with a hardware renderer, and keeps a streaming texture of `SCREEN_WIDTH` x `SCREEN_HEIGHT` (1280 x 720) pixels. Each frame, [Present](#windowpresent) copies a framebuffer, such as [Renderer.Pixels](Renderer.md#rendererpixels), into that texture and draws it to the window.

The window opens at twice the framebuffer size (2560 x 1440) and can be resized freely. The image is scaled with nearest-neighbour filtering and letterboxed, so pixels stay sharp and the aspect ratio never changes.

Initialise the window in this order: [Init](#windowinit), [InitRenderer](#windowinitrenderer), [InitTexture](#windowinittexture). [Run](../Application/Game.md#run) does all of this for you.

The destructor calls [Shutdown](#windowshutdown). `Window` cannot be copied.

```cpp
#include <Rendering/Window.h>
#include <Rendering/Renderer.h>

Window window;
window.title = "My Game";
if (!window.Init() || !window.InitRenderer() || !window.InitTexture()) { return 1; }
window.SetMouseCaptured(true);

Renderer renderer(SCREEN_WIDTH, SCREEN_HEIGHT);
// each frame:
renderer.Clear(0xFF000000);
window.Present(renderer.Pixels());
```

Additional resources: [Renderer](Renderer.md), [Game](../Application/Game.md), [BindingHotkeys](../InputSystem/BindingHotkeys.md).

## Properties

| Property | Description |
| --- | --- |
| [title](#windowtitle) | The window's normal title text. |

## Constants

| Constant | Description |
| --- | --- |
| [SCREEN_WIDTH](#screen_width-and-screen_height) | Framebuffer width, 1280 pixels. |
| [SCREEN_HEIGHT](#screen_width-and-screen_height) | Framebuffer height, 720 pixels. |

## Constructors

| Constructor | Description |
| --- | --- |
| [Window](#windowwindow) | Creates a window object without touching SDL. |

## Public Methods

| Method | Description |
| --- | --- |
| [Init](#windowinit) | Starts SDL's video and gamepad subsystems. |
| [InitRenderer](#windowinitrenderer) | Creates the window and its renderer. |
| [InitTexture](#windowinittexture) | Creates the texture that frames are copied into. |
| [Shutdown](#windowshutdown) | Destroys everything and shuts SDL down. |
| [Present](#windowpresent) | Shows a framebuffer in the window. |
| [SetTitle](#windowsettitle) | Changes the text in the title bar. |
| [SetMouseCaptured](#windowsetmousecaptured) | Turns relative mouse mode on or off. |

---

## Window.title

### Declaration

```cpp
std::string title = "BlueJay - Window (Version)";
```

### Description

The window's normal title text.

[InitRenderer](#windowinitrenderer) uses it when it creates the window, and [BindingHotkeys](../InputSystem/BindingHotkeys.md) restores it after a rebind prompt. Changing it later does not update the title bar on its own; call [SetTitle](#windowsettitle) as well.

---

## SCREEN_WIDTH and SCREEN_HEIGHT

### Declaration

```cpp
constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;
```

### Description

Size of the framebuffer the window presents, in pixels. Create your [Renderer](Renderer.md) with these values and use their ratio as the aspect ratio for [Perspective](../Core/Matrix4x4.md#perspective).

---

## Window.Window

### Declaration

```cpp
Window() = default;
```

### Description

Creates a window object. Nothing is created in SDL until [Init](#windowinit) and the calls after it.

---

## Window.Init

### Declaration

```cpp
bool Init();
```

### Returns

**bool** `true` on success. On failure, the SDL error is printed to standard error.

### Description

Starts SDL's video and gamepad subsystems. Call it first.

---

## Window.InitRenderer

### Declaration

```cpp
bool InitRenderer();
```

### Returns

**bool** `true` on success. On failure, the SDL error is printed to standard error.

### Description

Creates the window, titled with [title](#windowtitle), and an SDL renderer for it. The window is resizable and opens at twice `SCREEN_WIDTH` x `SCREEN_HEIGHT`. The renderer letterboxes a `SCREEN_WIDTH` x `SCREEN_HEIGHT` logical image into whatever size the window has.

Call it after [Init](#windowinit).

---

## Window.InitTexture

### Declaration

```cpp
bool InitTexture();
```

### Returns

**bool** `true` on success. On failure, the SDL error is printed to standard error.

### Description

Creates the streaming ARGB8888 texture, `SCREEN_WIDTH` x `SCREEN_HEIGHT`, that [Present](#windowpresent) copies frames into. It uses nearest-neighbour scaling so pixels stay sharp.

Call it after [InitRenderer](#windowinitrenderer).

---

## Window.Shutdown

### Declaration

```cpp
void Shutdown();
```

### Description

Destroys the texture, renderer and window, then calls `SDL_Quit`.

It is safe to call more than once, and it does nothing if [Init](#windowinit) never succeeded. The destructor calls it, so you rarely need to call it yourself. Shut down the [InputSystem](../InputSystem/InputSystem.md) before this runs.

---

## Window.Present

### Declaration

```cpp
void Present(const uint32_t* pixels);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `pixels` | `SCREEN_WIDTH * SCREEN_HEIGHT` pixels in `0xAARRGGBB` format, row by row from the top-left corner. |

### Description

Shows a framebuffer in the window: copies `pixels` into the texture, clears the window, draws the texture letterboxed, and presents.

The buffer must be exactly `SCREEN_WIDTH` x `SCREEN_HEIGHT`. A smaller buffer is read past its end.

---

## Window.SetTitle

### Declaration

```cpp
void SetTitle(const char* text);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `text` | New title bar text. |

### Description

Changes the text in the title bar. It does not change [title](#windowtitle), so code that restores the normal title still restores the old value.

---

## Window.SetMouseCaptured

### Declaration

```cpp
void SetMouseCaptured(bool captured);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `captured` | `true` to capture the mouse, `false` to release it. |

### Description

Turns SDL's relative mouse mode on or off. While captured, the cursor is hidden and kept inside the window, and mouse motion keeps reporting movement even at the screen edge, which suits mouse-look. Read the movement with [InputSystem.GetMouseDelta](../InputSystem/InputSystem.md#inputsystemgetmousedelta).
