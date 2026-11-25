# InputSystem

class in `InputSystem` / Implemented in: [`InputSystem/InputSystem.h`](../../../Source/Runtime/InputSystem/InputSystem.h)

## Description

Action-based keyboard, mouse and gamepad input on top of SDL3.

`InputSystem` is a singleton: get it with [getInstance](#inputsystemgetinstance). Game code asks about named actions ([InputAction](InputAction.md)) instead of physical keys, so players can rebind controls without the game code changing. Each action can have one key and one gamepad button, and it counts as down when either one is.

**Keyboard.** Keys are SDL scancodes, which name physical key positions, so bindings work the same on every keyboard layout.

**Gamepad.** The first gamepad connected is used. SDL's gamepad mapping gives every supported controller the same standard layout. Sticks are read with [GetStick](#inputsystemgetstick), and the analog triggers can be bound as buttons.

**Mouse.** [GetMouseDelta](#inputsystemgetmousedelta) reports how far the mouse moved this frame.

**Frames.** Events collected by [ProcessEvent](#inputsystemprocessevent) are published by [Update](#inputsystemupdate), so every query between two `Update` calls sees the same frame. When you use [Run](../Application/Game.md#run), all of this is done for you. If you write your own loop, do it like this:

```cpp
#include <InputSystem/InputSystem.h>
#include <SDL3/SDL.h>

namespace Inp = InputSystem;
Inp::InputSystem& input = Inp::InputSystem::getInstance();

input.Init();                       // after SDL_Init
input.LoadBindings("bindings.cfg");

while (running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) { input.ProcessEvent(e); }
    input.Update();                 // once per frame, before game logic

    if (input.IsActionPressed(Inp::ACTION_MOVE_JUMP)) { player.Jump(); }
    if (input.IsActionDown(Inp::ACTION_MOVE_FORWARD)) { player.MoveForward(dt); }

    float x, y;
    input.GetStick(false, x, y);    // left stick, deadzone applied
    player.Move(x, y, dt);
}

input.Shutdown();                   // before SDL_Quit
```

`InputSystem` cannot be copied or moved.

Additional resources: [InputAction](InputAction.md), [BindingHotkeys](BindingHotkeys.md), [CameraController](../Rendering/CameraController.md).

## Static Methods

| Method | Description |
| --- | --- |
| [getInstance](#inputsystemgetinstance) | Returns the one input system. |
| [KeyName](#inputsystemkeyname) | Returns the name of a key. |
| [ButtonName](#inputsystembuttonname) | Returns the name of a gamepad button. |

## Properties

| Property | Description |
| --- | --- |
| [inputActions](#inputsysteminputactions) | Every action, indexed by [ActionType](InputAction.md#actiontype). |

## Public Methods

| Method | Description |
| --- | --- |
| [Init](#inputsysteminit) | Starts SDL's gamepad subsystem. |
| [Shutdown](#inputsystemshutdown) | Closes the gamepad and stops SDL's gamepad subsystem. |
| [ProcessEvent](#inputsystemprocessevent) | Feeds one SDL event to the input system. |
| [Update](#inputsystemupdate) | Publishes this frame's input. Call once per frame. |
| [IsActionDown](#inputsystemisactiondown) | Whether an action is held. |
| [IsActionPressed](#inputsystemisactionpressed) | Whether an action was pressed this frame. |
| [IsActionReleased](#inputsystemisactionreleased) | Whether an action was released this frame. |
| [GetAction](#inputsystemgetaction) | Returns an action so it can be changed. |
| [SetDefaultActions](#inputsystemsetdefaultactions) | Creates every action that does not exist yet, with its default bindings. |
| [ResetToDefaults](#inputsystemresettodefaults) | Replaces every action with its default. |
| [SetAction](#inputsystemsetaction) | Changes an action's key and button. |
| [CreateAction](#inputsystemcreateaction) | Fills an unused action slot. |
| [GetPressedKeys](#inputsystemgetpressedkeys) | Keys pressed this frame. |
| [GetHeldKeys](#inputsystemgetheldkeys) | Keys held down this frame. |
| [GetLastPressedKey](#inputsystemgetlastpressedkey) | The last key pressed this frame. |
| [GetLastPressedButton](#inputsystemgetlastpressedbutton) | The last gamepad button pressed this frame. |
| [GetStick](#inputsystemgetstick) | Position of an analog stick. |
| [GetMouseDelta](#inputsystemgetmousedelta) | Mouse movement this frame. |
| [HasGamepad](#inputsystemhasgamepad) | Whether a gamepad is connected. |
| [BeginRebind](#inputsystembeginrebind) | Makes the next key or button pressed an action's new binding. |
| [CancelRebind](#inputsystemcancelrebind) | Stops waiting for a rebind. |
| [IsRebinding](#inputsystemisrebinding) | Whether a rebind is waiting for input. |
| [GetRebindAction](#inputsystemgetrebindaction) | The action being rebound. |
| [SaveBindings](#inputsystemsavebindings) | Writes every binding to a text file. |
| [LoadBindings](#inputsystemloadbindings) | Reads bindings from a text file. |

---

## InputSystem.getInstance

### Declaration

```cpp
static InputSystem& getInstance();
```

### Returns

**InputSystem&** The one input system.

### Description

Returns the one input system, creating it on first use.

On creation every action is set up with its default bindings (see [SetDefaultActions](#inputsystemsetdefaultactions)). It does not touch SDL until [Init](#inputsysteminit) is called. The instance lives until the program exits, which is after SDL has shut down, so call [Shutdown](#inputsystemshutdown) yourself before `SDL_Quit`.

---

## InputSystem.inputActions

### Declaration

```cpp
std::vector<InputAction> inputActions;
```

### Description

Every action, indexed by [ActionType](InputAction.md#actiontype). The vector has `MAX_ACTION` entries, and entry 0 (`NO_ACTION`) is never used.

Prefer [GetAction](#inputsystemgetaction), which checks the index for you.

---

## InputSystem.Init

### Declaration

```cpp
void Init();
```

### Description

Starts SDL's gamepad subsystem. Call it after `SDL_Init`.

Gamepads that are already plugged in are announced by SDL as "added" events, which [ProcessEvent](#inputsystemprocessevent) handles, so they are picked up on the first frames.

---

## InputSystem.Shutdown

### Declaration

```cpp
void Shutdown();
```

### Description

Closes the open gamepad, if any, and stops SDL's gamepad subsystem. Call it before `SDL_Quit`.

---

## InputSystem.ProcessEvent

### Declaration

```cpp
void ProcessEvent(const SDL_Event& e);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `e` | An event from `SDL_PollEvent`. |

### Description

Feeds one SDL event to the input system. Pass every event; ones it does not use are ignored.

It handles:

* **Gamepad added**: opens the gamepad if none is open yet.
* **Gamepad removed**: closes it if it is the open one.
* **Key down**: queued as pressed. Key repeats are ignored. While a keyboard rebind is waiting, the key becomes the new binding instead (Esc cancels), and it is not reported as pressed.
* **Key up**: queued as released.
* **Mouse motion**: added to this frame's mouse delta.

Nothing is visible to the queries until the next [Update](#inputsystemupdate).

When you use [BindingHotkeys](BindingHotkeys.md), call its `ProcessEvent` instead. It forwards the event here.

---

## InputSystem.Update

### Declaration

```cpp
void Update();
```

### Description

Publishes this frame's input. Call it once per frame, after handling events and before your game logic.

It makes the keys and mouse movement queued by [ProcessEvent](#inputsystemprocessevent) the current frame's, reads which keys are held, and polls the gamepad's buttons and triggers to work out which were pressed and released since the last call. If a gamepad rebind is waiting, the first button pressed this frame becomes the new binding.

---

## InputSystem.IsActionDown

### Declaration

```cpp
bool IsActionDown(int action);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `action` | An [ActionType](InputAction.md#actiontype) value. |

### Returns

**bool** `true` while the action's key or button is held.

### Description

Whether an action is held. Use it for continuous input such as movement.

Returns `false` for an invalid action or one whose [isEnabled](InputAction.md#inputactionisenabled) is `false`. The key is read from SDL's live keyboard state; the button is read from the state captured by the last [Update](#inputsystemupdate).

---

## InputSystem.IsActionPressed

### Declaration

```cpp
bool IsActionPressed(int action);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `action` | An [ActionType](InputAction.md#actiontype) value. |

### Returns

**bool** `true` on the frame the action's key or button went down.

### Description

Whether an action was pressed this frame. Use it for one-off input such as jumping or opening a menu.

It is `true` for exactly one frame per press. Returns `false` for an invalid or disabled action.

```cpp
if (input.IsActionPressed(InputSystem::ACTION_USE_FLASHLIGHT)) {
    flashlightOn = !flashlightOn;
}
```

---

## InputSystem.IsActionReleased

### Declaration

```cpp
bool IsActionReleased(int action);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `action` | An [ActionType](InputAction.md#actiontype) value. |

### Returns

**bool** `true` on the frame the action's key or button came up.

### Description

Whether an action was released this frame. Returns `false` for an invalid or disabled action.

---

## InputSystem.GetAction

### Declaration

```cpp
InputAction* GetAction(int action);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `action` | An [ActionType](InputAction.md#actiontype) value. |

### Returns

**InputAction\*** The action, or `nullptr` when `action` is `NO_ACTION` or out of range.

### Description

Returns an action so it can be read or changed, for example to turn it off or show its name in a menu.

---

## InputSystem.SetDefaultActions

### Declaration

```cpp
void SetDefaultActions(void);
```

### Description

Creates every action that does not exist yet, with its default name, key and button. The defaults are listed under [ActionType](InputAction.md#actiontype).

Actions that already exist are left alone (and a message is printed for each), so to restore the defaults use [ResetToDefaults](#inputsystemresettodefaults). The constructor calls this once.

---

## InputSystem.ResetToDefaults

### Declaration

```cpp
void ResetToDefaults(void);
```

### Description

Replaces every action with its default: clears all actions, including any changes to `name` and `isEnabled`, then calls [SetDefaultActions](#inputsystemsetdefaultactions). Call [SaveBindings](#inputsystemsavebindings) afterwards to keep the result.

---

## InputSystem.SetAction

### Declaration

```cpp
void SetAction(int action, int key, int button);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `action` | An [ActionType](InputAction.md#actiontype) value. |
| `key` | SDL scancode, or `KEY_NULL` for none. |
| `button` | Gamepad button, trigger pseudo-button, or `GAMEPAD_BUTTON_NONE`. |

### Description

Changes an action's key and button. Prints `Unknown Action` to standard output and does nothing if `action` is invalid.

---

## InputSystem.CreateAction

### Declaration

```cpp
void CreateAction(int action, const std::string& name, int key, int button);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `action` | An [ActionType](InputAction.md#actiontype) value. |
| `name` | Display name, also used in bindings files. |
| `key` | SDL scancode, or `KEY_NULL` for none. |
| `button` | Gamepad button, trigger pseudo-button, or `GAMEPAD_BUTTON_NONE`. |

### Description

Fills an unused action slot with an id, name and bindings.

Does nothing if the slot already has an action (it prints `Existing Action`) or if `action` is invalid (it prints `Invalid Action`). To change an existing action, use [SetAction](#inputsystemsetaction) or [GetAction](#inputsystemgetaction).

---

## InputSystem.GetPressedKeys

### Declaration

```cpp
const std::vector<int>& GetPressedKeys() const;
```

### Returns

**const std::vector\<int\>&** Scancodes of the keys pressed this frame, in the order they were pressed.

### Description

Keys pressed this frame, whatever actions they are bound to. Useful for text-like input or debug keys.

---

## InputSystem.GetHeldKeys

### Declaration

```cpp
const std::vector<int>& GetHeldKeys() const;
```

### Returns

**const std::vector\<int\>&** Scancodes of every key held down when [Update](#inputsystemupdate) last ran, in scancode order.

### Description

Keys held down this frame.

---

## InputSystem.GetLastPressedKey

### Declaration

```cpp
int GetLastPressedKey() const;
```

### Returns

**int** The scancode of the last key pressed this frame, or `KEY_NULL` if none was.

### Description

The last key pressed this frame.

---

## InputSystem.GetLastPressedButton

### Declaration

```cpp
int GetLastPressedButton() const;
```

### Returns

**int** The last gamepad button pressed this frame, or `GAMEPAD_BUTTON_NONE` if none was. Buttons are checked in ascending order, so when several go down in the same frame this is the highest-numbered one.

### Description

The last gamepad button pressed this frame.

---

## InputSystem.GetStick

### Declaration

```cpp
void GetStick(bool rightStick, float& x, float& y) const;
```

### Parameters

| Parameter | Description |
| --- | --- |
| `rightStick` | `true` for the right stick, `false` for the left stick. |
| `x` | Receives the horizontal position, from -1 (left) to 1 (right). |
| `y` | Receives the vertical position, from -1 (down) to 1 (up). |

### Description

Position of an analog stick, with a radial deadzone.

Positions within 15% of the centre read as `(0, 0)`. Beyond that the magnitude is rescaled so it rises smoothly from 0 at the edge of the deadzone to 1 at full tilt, keeping the stick's direction. Up is positive, unlike SDL's raw axis.

Both values are 0 when no gamepad is connected.

---

## InputSystem.GetMouseDelta

### Declaration

```cpp
void GetMouseDelta(float& dx, float& dy) const;
```

### Parameters

| Parameter | Description |
| --- | --- |
| `dx` | Receives horizontal movement this frame. Positive is right. |
| `dy` | Receives vertical movement this frame. Positive is down. |

### Description

How far the mouse moved this frame, in mouse counts (SDL's relative motion), summed over every motion event since the previous [Update](#inputsystemupdate).

This is a per-frame distance, not a speed, so do not multiply it by delta time.

---

## InputSystem.HasGamepad

### Declaration

```cpp
bool HasGamepad() const;
```

### Returns

**bool** `true` while a gamepad is open.

### Description

Whether a gamepad is connected. Use it to switch on-screen prompts between keyboard and gamepad icons.

---

## InputSystem.BeginRebind

### Declaration

```cpp
void BeginRebind(int action, bool forGamepad = false);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `action` | The [ActionType](InputAction.md#actiontype) to rebind. |
| `forGamepad` | `false` to wait for a key, `true` to wait for a gamepad button. |

### Description

Makes the next key or button pressed the action's new binding.

While a keyboard rebind waits, the key press that completes it is consumed and does not trigger any action. Esc cancels, so Esc cannot be chosen as a key. In gamepad mode, keyboard presses other than Esc are swallowed until a button is pressed, and the button that completes the rebind is *not* consumed: it still counts as pressed on that frame. Several actions may share a binding; nothing stops you choosing a key another action already uses.

The new binding is not saved automatically. Call [SaveBindings](#inputsystemsavebindings) once [IsRebinding](#inputsystemisrebinding) turns `false`, or use [BindingHotkeys](BindingHotkeys.md), which does it for you.

Does nothing if `action` is invalid.

```cpp
input.BeginRebind(InputSystem::ACTION_MOVE_JUMP);
// ... later, each frame:
if (wasRebinding && !input.IsRebinding()) { input.SaveBindings("bindings.cfg"); }
```

---

## InputSystem.CancelRebind

### Declaration

```cpp
void CancelRebind();
```

### Description

Stops waiting for a rebind. The action keeps its old binding.

---

## InputSystem.IsRebinding

### Declaration

```cpp
bool IsRebinding() const;
```

### Returns

**bool** `true` while a rebind is waiting for a key or button.

### Description

Whether a rebind is waiting for input.

---

## InputSystem.GetRebindAction

### Declaration

```cpp
int GetRebindAction() const;
```

### Returns

**int** The [ActionType](InputAction.md#actiontype) passed to the last [BeginRebind](#inputsystembeginrebind) call. Only meaningful while [IsRebinding](#inputsystemisrebinding) is `true`.

### Description

The action being rebound.

---

## InputSystem.SaveBindings

### Declaration

```cpp
bool SaveBindings(const char* path) const;
```

### Parameters

| Parameter | Description |
| --- | --- |
| `path` | File to write. An existing file is replaced. |

### Returns

**bool** `true` if the file was written.

### Description

Writes every binding to a text file, one action per line in the form `Name = Key | Button`:

```text
# BlueJay input bindings:  Action = Key | GamepadButton
# Key names come from SDL_GetScancodeName, button names from SDL's gamepad API.
Move Forward = W | dpup
Jump = Space | a
Item_Left = T | lefttrigger
```

Key names come from [KeyName](#inputsystemkeyname) and button names from [ButtonName](#inputsystembuttonname). An unbound key or button is written as `none`.

---

## InputSystem.LoadBindings

### Declaration

```cpp
bool LoadBindings(const char* path);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `path` | File to read, in the format [SaveBindings](#inputsystemsavebindings) writes. |

### Returns

**bool** `false` if the file could not be opened, otherwise `true`.

### Description

Reads bindings from a text file and applies them on top of the current ones.

The format is forgiving, so files from older or newer builds keep loading:

* Blank lines and lines starting with `#` are skipped.
* Lines naming an action that does not exist are skipped.
* `none` clears the key or button.
* A key or button name SDL does not recognise keeps the action's current binding.
* Actions missing from the file keep their current bindings.

Each line is split at its first `=` and its last `|`, so key names that contain those characters still load.

---

## InputSystem.KeyName

### Declaration

```cpp
static std::string KeyName(int key);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `key` | SDL scancode. |

### Returns

**std::string** SDL's name for the key, such as `"W"` or `"Left Shift"`, or `"none"` for `KEY_NULL`, an out-of-range value, or a key SDL has no name for.

### Description

Returns the name of a key, for menus and bindings files.

---

## InputSystem.ButtonName

### Declaration

```cpp
static std::string ButtonName(int button);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `button` | Gamepad button or trigger pseudo-button. |

### Returns

**std::string** SDL's name for the button, such as `"a"` or `"dpup"`; `"lefttrigger"` or `"righttrigger"` for the trigger pseudo-buttons; or `"none"` for anything else.

### Description

Returns the name of a gamepad button, for menus and bindings files.
