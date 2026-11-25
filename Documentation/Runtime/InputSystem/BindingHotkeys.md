# BindingHotkeys

class in `InputSystem` / Implemented in: [`InputSystem/BindingHotkeys.h`](../../../Source/Runtime/InputSystem/BindingHotkeys.h)

## Description

Lets the player rebind actions with the function keys, with no menu needed.

You give `BindingHotkeys` a list of actions. Pressing F1 starts a keyboard rebind for the first action, F2 for the second, and so on. While a rebind waits, the window title shows `Press a key for <action name> (Esc cancels)`. When the player presses a key, or Esc to cancel, every binding is saved to the bindings file and the title goes back to normal.

[Run](../Application/Game.md#run) creates one from [AppConfig.rebindable](../Application/AppConfig.md#appconfigrebindable) and [AppConfig.bindingsFile](../Application/AppConfig.md#appconfigbindingsfile), so you only need this class if you write your own main loop.

Use at most 12 actions. SDL numbers F1 to F12 consecutively, but the keys after F12 in SDL's numbering are Print Screen, Scroll Lock, Pause and so on, so a 13th action would be rebound by Print Screen.

```cpp
#include <InputSystem/BindingHotkeys.h>
#include <InputSystem/InputSystem.h>
#include <Rendering/Window.h>
#include <SDL3/SDL.h>

namespace Inp = InputSystem;

Window window;
// ... window.Init(), InitRenderer(), InitTexture() ...

Inp::BindingHotkeys hotkeys({ Inp::ACTION_MOVE_FORWARD, Inp::ACTION_MOVE_JUMP }, "bindings.cfg");

SDL_Event e;
while (SDL_PollEvent(&e)) {
    hotkeys.ProcessEvent(e, window);   // instead of InputSystem::ProcessEvent
}
Inp::InputSystem::getInstance().Update();
```

Additional resources: [InputSystem](InputSystem.md), [AppConfig](../Application/AppConfig.md), [Window](../Rendering/Window.md).

## Constructors

| Constructor | Description |
| --- | --- |
| [BindingHotkeys](#bindinghotkeysbindinghotkeys) | Creates hotkeys for a list of actions and a bindings file. |

## Public Methods

| Method | Description |
| --- | --- |
| [ProcessEvent](#bindinghotkeysprocessevent) | Handles one SDL event and forwards it to the input system. |

---

## BindingHotkeys.BindingHotkeys

### Declaration

```cpp
BindingHotkeys(std::vector<int> actions, std::string bindingsFile);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `actions` | [ActionType](InputAction.md#actiontype) values. F1 rebinds the first, F2 the second, and so on. |
| `bindingsFile` | File the bindings are saved to after each rebind. |

### Description

Creates hotkeys for a list of actions and a bindings file.

---

## BindingHotkeys.ProcessEvent

### Declaration

```cpp
void ProcessEvent(const SDL_Event& e, Window& window);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `e` | An event from `SDL_PollEvent`. |
| `window` | Window whose title shows the rebind prompt. |

### Description

Handles one SDL event and forwards it to the input system. Call it for every event instead of [InputSystem.ProcessEvent](InputSystem.md#inputsystemprocessevent).

For each event it:

1. Forwards the event to [InputSystem.ProcessEvent](InputSystem.md#inputsystemprocessevent).
2. If that event finished a rebind (a key was chosen or Esc cancelled), saves all bindings with [InputSystem.SaveBindings](InputSystem.md#inputsystemsavebindings) and sets the window title back to [Window.title](../Rendering/Window.md#windowtitle).
3. If no rebind was in progress and the event is a fresh press of F1 + *n*, where *n* is less than the number of actions, starts a keyboard rebind for action *n* with [InputSystem.BeginRebind](InputSystem.md#inputsystembeginrebind) and shows the prompt in the title bar.

Only keyboard rebinds are started. The function key that starts a rebind is also reported to the game as a normal key press.
