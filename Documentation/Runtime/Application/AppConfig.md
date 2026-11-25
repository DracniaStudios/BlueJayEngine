# AppConfig

struct in global namespace / Implemented in: [`Application/Application.h`](../../../Source/Runtime/Application/Application.h)

## Description

Settings for the application that [Run](Game.md#run) creates.

Fill in an `AppConfig` and pass it to `Run` along with your [Game](Game.md). Every field has a default, so a default-constructed `AppConfig` is enough to get a window on screen.

```cpp
#include <Application/Application.h>
#include <InputSystem/InputSystem.h>

int main() {
    namespace Inp = InputSystem;

    AppConfig config;
    config.title = "My Game";
    config.bindingsFile = "controls.cfg";
    config.rebindable = { Inp::ACTION_MOVE_FORWARD,     // F1
                          Inp::ACTION_MOVE_BACKWARD,    // F2
                          Inp::ACTION_MOVE_JUMP };      // F3
    config.captureMouse = true;

    MyGame game;
    return Run(config, game);
}
```

Additional resources: [Game](Game.md), [BindingHotkeys](../InputSystem/BindingHotkeys.md), [InputSystem](../InputSystem/InputSystem.md).

## Properties

| Property | Description |
| --- | --- |
| [title](#appconfigtitle) | Text in the window's title bar. |
| [bindingsFile](#appconfigbindingsfile) | File the input bindings are loaded from and saved to. |
| [rebindable](#appconfigrebindable) | Actions that F1, F2, F3 and so on start rebinding, in order. |
| [captureMouse](#appconfigcapturemouse) | Whether the window captures the mouse in relative mode. |

---

## AppConfig.title

### Declaration

```cpp
std::string title = "BlueJay";
```

### Description

Text in the window's title bar.

While a rebind started by [BindingHotkeys](../InputSystem/BindingHotkeys.md) is waiting for a key, the title shows a prompt instead, and switches back to this text when the rebind finishes.

---

## AppConfig.bindingsFile

### Declaration

```cpp
std::string bindingsFile = "bindings.cfg";
```

### Description

File the input bindings are loaded from and saved to.

[Run](Game.md#run) calls [InputSystem.LoadBindings](../InputSystem/InputSystem.md#inputsystemloadbindings) with this path at startup. A missing file is not an error: the default bindings stay in place. After every rebind, [BindingHotkeys](../InputSystem/BindingHotkeys.md) saves all bindings back to this path. A relative path is resolved against the working directory.

---

## AppConfig.rebindable

### Declaration

```cpp
std::vector<int> rebindable;
```

### Description

Actions that F1, F2, F3 and so on start rebinding, in order.

Each entry is an [ActionType](../InputSystem/InputAction.md#actiontype) value. Pressing F1 starts a rebind for the first entry, F2 for the second, and so on. The next key the player presses becomes that action's key, and Esc cancels. Leave the list empty to turn rebinding hotkeys off.

Keep the list to 12 entries at most. See [BindingHotkeys](../InputSystem/BindingHotkeys.md) for why.

---

## AppConfig.captureMouse

### Declaration

```cpp
bool captureMouse = true;
```

### Description

Whether the window captures the mouse in relative mode.

When `true`, the cursor is hidden and confined to the window, and [InputSystem.GetMouseDelta](../InputSystem/InputSystem.md#inputsystemgetmousedelta) reports unbounded motion, which suits a mouse-look camera. Set it to `false` for games that need a visible cursor.
