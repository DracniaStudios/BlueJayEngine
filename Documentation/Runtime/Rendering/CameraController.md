# CameraController

class in global namespace / Implemented in: [`Rendering/CameraController.h`](../../../Source/Runtime/Rendering/CameraController.h)

## Description

Flies a [Camera](Camera.md) from player input.

Call [Update](#cameracontrollerupdate) once per frame with the camera to move. The controller reads the [InputSystem](../InputSystem/InputSystem.md) and changes the camera's look angles and position. The controller stores only tuning values, so one controller can drive any number of cameras.

| Input | Effect |
| --- | --- |
| Mouse, right stick | Look around. |
| Move Forward / Backward, left stick up and down | Fly along the look direction, including up and down. |
| Move Left / Right, left stick left and right | Strafe horizontally. |
| Editor_Up / Editor_Down | Rise and sink along world Y. |
| Sprint (held) | Multiply speed by [sprintMultiplier](#cameracontrollersprintmultiplier). |

Moving diagonally is never faster than moving along one axis.

```cpp
#include <Rendering/Camera.h>
#include <Rendering/CameraController.h>
#include <InputSystem/InputSystem.h>

class MyGame : public Game {
    Camera camera_;
    CameraController controller_;

public:
    MyGame() {
        controller_.moveSpeed = 8.0f;
        controller_.mouseSens = 0.002f;
    }

    bool Update(float dt) override {
        controller_.Update(camera_, InputSystem::InputSystem::getInstance(), dt);
        return true;
    }
    // ...
};
```

Additional resources: [Camera](Camera.md), [InputSystem](../InputSystem/InputSystem.md).

## Properties

| Property | Description |
| --- | --- |
| [moveSpeed](#cameracontrollermovespeed) | Movement speed, in units per second. |
| [sprintMultiplier](#cameracontrollersprintmultiplier) | Speed multiplier while Sprint is held. |
| [mouseSens](#cameracontrollermousesens) | Look rotation per mouse count, in radians. |
| [stickLookSpeed](#cameracontrollersticklookspeed) | Look speed at full right-stick tilt, in radians per second. |

## Public Methods

| Method | Description |
| --- | --- |
| [Update](#cameracontrollerupdate) | Applies one frame of look and movement input to a camera. |

---

## CameraController.moveSpeed

### Declaration

```cpp
float moveSpeed = 4.0f;
```

### Description

Movement speed at full input, in world units per second.

---

## CameraController.sprintMultiplier

### Declaration

```cpp
float sprintMultiplier = 2.0f;
```

### Description

Factor applied to [moveSpeed](#cameracontrollermovespeed) while the Sprint action is held.

---

## CameraController.mouseSens

### Declaration

```cpp
float mouseSens = 0.0025f;
```

### Description

Look rotation per mouse count, in radians. Mouse movement is a per-frame distance, so it is not scaled by delta time.

---

## CameraController.stickLookSpeed

### Declaration

```cpp
float stickLookSpeed = 2.5f;
```

### Description

Look speed at full right-stick tilt, in radians per second. Stick input is a rate, so it is scaled by delta time.

---

## CameraController.Update

### Declaration

```cpp
void Update(Camera& camera, InputSystem::InputSystem& input, float dt) const;
```

### Parameters

| Parameter | Description |
| --- | --- |
| `camera` | The camera to move. |
| `input` | The input system to read, normally `InputSystem::getInstance()`. |
| `dt` | Seconds since the previous frame. |

### Description

Applies one frame of look and movement input to a camera.

Look input is applied first, through [Camera.Look](Camera.md#cameralook), so pitch stays clamped. Moving the mouse right or pushing the right stick right turns right, and moving the mouse up or pushing the stick up looks up.

Movement is then built from the action bindings plus the left stick. If the combined input is longer than 1 it is normalised, then it is multiplied by the speed and `dt` and added to [Camera.position](Camera.md#cameraposition). Forward and backward follow [Camera.Forward](Camera.md#cameraforward), strafing follows [Camera.Right](Camera.md#cameraright), and Editor_Up / Editor_Down move along world Y.

Call it after [InputSystem.Update](../InputSystem/InputSystem.md#inputsystemupdate), for example from [Game.Update](../Application/Game.md#gameupdate).
