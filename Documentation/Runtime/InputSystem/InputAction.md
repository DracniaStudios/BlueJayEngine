# InputAction

struct in `InputSystem` / Implemented in: [`InputSystem/InputSystem.h`](../../../Source/Runtime/InputSystem/InputSystem.h)

## Description

A named game action and the key and gamepad button bound to it.

Game code asks about actions, such as "Jump", rather than about physical keys. The [InputSystem](InputSystem.md) keeps one `InputAction` per [ActionType](#actiontype) in [InputSystem.inputActions](InputSystem.md#inputsysteminputactions), and an action counts as down when either its key or its button is down.

Get an action with [InputSystem.GetAction](InputSystem.md#inputsystemgetaction) to rename it, rebind it or turn it off.

```cpp
#include <InputSystem/InputSystem.h>

namespace Inp = InputSystem;
auto& input = Inp::InputSystem::getInstance();

if (Inp::InputAction* jump = input.GetAction(Inp::ACTION_MOVE_JUMP)) {
    jump->isEnabled = false;   // ignore jump during a cutscene
}
```

Additional resources: [InputSystem](InputSystem.md), [BindingHotkeys](BindingHotkeys.md).

## Properties

| Property | Description |
| --- | --- |
| [name](#inputactionname) | Display name, also used as the action's key in a bindings file. |
| [isEnabled](#inputactionisenabled) | Whether the action reports input at all. |
| [id](#inputactionid) | The action's [ActionType](#actiontype), or `NO_ACTION` for an unused slot. |
| [key](#inputactionkey) | SDL scancode of the bound key, or `KEY_NULL`. |
| [button](#inputactionbutton) | Bound gamepad button, or `GAMEPAD_BUTTON_NONE`. |

## Related Types and Constants

| Name | Description |
| --- | --- |
| [ActionType](#actiontype) | Every action the engine defines. |
| [Binding constants](#binding-constants) | Special values for `key` and `button`. |

---

## InputAction.name

### Declaration

```cpp
std::string name = "";
```

### Description

Display name, such as `"Move Forward"`.

[InputSystem.SaveBindings](InputSystem.md#inputsystemsavebindings) writes it at the start of each line, and [InputSystem.LoadBindings](InputSystem.md#inputsystemloadbindings) matches lines to actions by it. Renaming an action therefore makes older bindings files skip that action. Names must not contain `=`.

---

## InputAction.isEnabled

### Declaration

```cpp
bool isEnabled = true;
```

### Description

Whether the action reports input at all.

While `false`, [IsActionDown](InputSystem.md#inputsystemisactiondown), [IsActionPressed](InputSystem.md#inputsystemisactionpressed) and [IsActionReleased](InputSystem.md#inputsystemisactionreleased) return `false` for this action, whatever its bindings are doing.

---

## InputAction.id

### Declaration

```cpp
int id = NO_ACTION;
```

### Description

The action's [ActionType](#actiontype). It matches the action's index in [InputSystem.inputActions](InputSystem.md#inputsysteminputactions).

`NO_ACTION` marks a slot that has not been created yet. [InputSystem.CreateAction](InputSystem.md#inputsystemcreateaction) sets it, and only for a slot that is still `NO_ACTION`.

---

## InputAction.key

### Declaration

```cpp
int key = KEY_NULL;
```

### Description

SDL scancode of the bound key, or `KEY_NULL` (0) for no key.

Scancodes name physical key positions, so the same binding works on any keyboard layout: `SDL_SCANCODE_W` is always the key in the W position of a US keyboard, even on an AZERTY keyboard where that key is labelled Z.

---

## InputAction.button

### Declaration

```cpp
int button = GAMEPAD_BUTTON_NONE;
```

### Description

Bound gamepad button, or `GAMEPAD_BUTTON_NONE` (-1) for no button.

The value is an `SDL_GamepadButton`, which uses SDL's standard layout for every controller (for example `SDL_GAMEPAD_BUTTON_SOUTH` is A on an Xbox pad and Cross on a PlayStation pad). It can also be `GAMEPAD_TRIGGER_LEFT` or `GAMEPAD_TRIGGER_RIGHT` to treat an analog trigger as a button.

---

## ActionType

### Declaration

```cpp
enum ActionType : uint8_t { NO_ACTION = 0, /* ... */ MAX_ACTION };
```

### Description

Every action the engine defines, and the index of each in [InputSystem.inputActions](InputSystem.md#inputsysteminputactions). The table shows the bindings [InputSystem.SetDefaultActions](InputSystem.md#inputsystemsetdefaultactions) creates.

| Value | Name | Default key | Default gamepad button |
| --- | --- | --- | --- |
| `NO_ACTION` | — | — | — |
| `ACTION_MOVE_FORWARD` | Move Forward | W | D-pad up |
| `ACTION_MOVE_BACKWARD` | Move Backward | S | D-pad down |
| `ACTION_MOVE_RIGHT` | Move Right | D | D-pad right |
| `ACTION_MOVE_LEFT` | Move Left | A | D-pad left |
| `ACTION_MOVE_CROUCH` | Crouch | Left Ctrl | East (B) |
| `ACTION_MOVE_SPRINT` | Sprint | Left Shift | Left stick click |
| `ACTION_MOVE_INTERACT` | Interact | F | West (X) |
| `ACTION_MOVE_JUMP` | Jump | Space | South (A) |
| `ACTION_USE_ARTIFACT` | Artifact_Interact | E | Back |
| `ACTION_USE_ARTIFACT_RIGHT` | Artifact_Right | R | Right stick click |
| `ACTION_USE_ARTIFACT_LEFT` | Artifact_Left | Q | Guide |
| `ACTION_USE_ITEM` | Item_Left | T | Left trigger |
| `ACTION_USE_ITEM2` | Item_Right | G | Right trigger |
| `ACTION_USE_FLASHLIGHT` | Flashlight | L | North (Y) |
| `ACTION_EDITOR_UP` | Editor_Up | Space | Right shoulder |
| `ACTION_EDITOR_DOWN` | Editor_Down | Left Alt | Left shoulder |
| `ACTION_UI_BACK` | Back | Tab | Back |
| `ACTION_UI_PAUSE` | Pause | Esc | Start |
| `MAX_ACTION` | — | — | — |

`MAX_ACTION` is the number of slots, not an action. Several actions share a binding on purpose, for example Jump and Editor_Up both use Space.

---

## Binding constants

### Declaration

```cpp
constexpr int KEY_NULL = 0;
constexpr int kMaxKeys = 512;
constexpr int GAMEPAD_BUTTON_NONE = -1;
constexpr int GAMEPAD_TRIGGER_LEFT = 100;
constexpr int GAMEPAD_TRIGGER_RIGHT = 101;
constexpr int kMaxButtons = 128;
```

### Description

| Constant | Description |
| --- | --- |
| `KEY_NULL` | No key bound. Equal to `SDL_SCANCODE_UNKNOWN`. |
| `kMaxKeys` | Upper bound for scancodes. Keys at or above it are ignored. Checked at compile time to cover every SDL scancode. |
| `GAMEPAD_BUTTON_NONE` | No gamepad button bound. |
| `GAMEPAD_TRIGGER_LEFT` | Pseudo-button for the left analog trigger. It is down while the trigger is pulled more than halfway. |
| `GAMEPAD_TRIGGER_RIGHT` | Pseudo-button for the right analog trigger, with the same threshold. |
| `kMaxButtons` | Upper bound for button values, including the trigger pseudo-buttons. |
