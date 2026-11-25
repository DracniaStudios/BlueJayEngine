# Camera

class in global namespace / Implemented in: [`Rendering/Camera.h`](../../../Source/Runtime/Rendering/Camera.h)

## Description

A first-person camera described by a position and two look angles.

`yaw` turns the camera left and right around the world's Y axis, and `pitch` tilts it up and down. With both at 0 the camera looks down -Z with +Y up. There is no roll.

Pass [View](#cameraview) to [Renderer.DrawMesh](Renderer.md#rendererdrawmesh) as the view matrix. To move the camera from player input, use a [CameraController](CameraController.md).

```cpp
#include <Rendering/Camera.h>

Camera camera;                     // at (0, 0, 7), looking down -Z
camera.Look(0.3f, -0.1f);          // turn left a little, tilt down a little
camera.position = camera.position + camera.Forward() * 2.0f;   // step forward

const Matrix4x4 view = camera.View();
const Vector3 lightDir = camera.ToViewSpace(Normalize({ 0.4f, 0.8f, 0.6f }));
renderer.DrawMesh(mesh, model, view, proj, lightDir);
```

Additional resources: [CameraController](CameraController.md), [Renderer](Renderer.md), [Matrix4x4](../Core/Matrix4x4.md).

## Properties

| Property | Description |
| --- | --- |
| [position](#cameraposition) | Position of the camera in world space. |
| [yaw](#camerayaw) | Horizontal look angle, in radians. |
| [pitch](#camerapitch) | Vertical look angle, in radians. |

## Public Methods

| Method | Description |
| --- | --- |
| [Forward](#cameraforward) | Unit vector the camera is looking along. |
| [Right](#cameraright) | Unit vector to the camera's right, kept horizontal. |
| [Look](#cameralook) | Turns the camera, keeping pitch short of straight up or down. |
| [ToViewSpace](#cameratoviewspace) | Rotates a world-space direction into view space. |
| [View](#cameraview) | The view matrix for rendering. |

---

## Camera.position

### Declaration

```cpp
Vector3 position{ 0, 0, 7 };
```

### Description

Position of the camera in world space. It starts 7 units along +Z, so a camera with default settings sees objects placed at the origin.

---

## Camera.yaw

### Declaration

```cpp
float yaw = 0.0f;
```

### Description

Horizontal look angle around the world Y axis, in radians. 0 looks down -Z, and increasing the value turns the camera to the left. It is not wrapped, so it can grow past ±2π.

---

## Camera.pitch

### Declaration

```cpp
float pitch = 0.0f;
```

### Description

Vertical look angle, in radians. 0 looks level and positive values look up. [Look](#cameralook) keeps it within ±1.55 radians (about ±89°). If you set it directly, keep it inside ±π/2.

---

## Camera.Forward

### Declaration

```cpp
Vector3 Forward() const;
```

### Returns

**Vector3** Unit vector the camera is looking along, including pitch.

### Description

Unit vector the camera is looking along. Moving along it flies the camera toward wherever it points, including up or down.

---

## Camera.Right

### Declaration

```cpp
Vector3 Right() const;
```

### Returns

**Vector3** Unit vector to the camera's right, with a Y component of 0.

### Description

Unit vector to the camera's right. It depends only on [yaw](#camerayaw), so strafing stays level however far the camera is pitched.

---

## Camera.Look

### Declaration

```cpp
void Look(float dYaw, float dPitch);
```

### Parameters

| Parameter | Description |
| --- | --- |
| `dYaw` | Change in yaw, in radians. Positive turns left. |
| `dPitch` | Change in pitch, in radians. Positive looks up. |

### Description

Turns the camera by the given angles.

Pitch is clamped to ±1.55 radians, just short of straight up or down, so the view never flips over.

---

## Camera.ToViewSpace

### Declaration

```cpp
Vector3 ToViewSpace(Vector3 direction) const;
```

### Parameters

| Parameter | Description |
| --- | --- |
| `direction` | A direction in world space. |

### Returns

**Vector3** The same direction expressed in the camera's view space.

### Description

Rotates a world-space direction into view space. The camera's position is ignored, so use it only for directions, not points.

[Renderer.DrawMesh](Renderer.md#rendererdrawmesh) expects its light direction in view space. Convert a world-space light direction with this function every frame so the light stays fixed in the world while the camera moves.

---

## Camera.View

### Declaration

```cpp
Matrix4x4 View() const;
```

### Returns

**Matrix4x4** The matrix that transforms world space into view space.

### Description

The view matrix for rendering: the inverse of the camera's world transform `Translation(position) * RotationY(yaw) * RotationX(pitch)`. Pass it to [Renderer.DrawMesh](Renderer.md#rendererdrawmesh).
