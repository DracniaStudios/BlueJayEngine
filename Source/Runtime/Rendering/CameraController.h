#pragma once
#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include <Rendering/Camera.h>

namespace InputSystem { class InputSystem; }

// Fly camera: mouse / right stick looks, move actions / left stick moves.
class CameraController {
public:
	float moveSpeed = 4.0f;
	float sprintMultiplier = 2.0f;
	float mouseSens = 0.0025f;     // radians per mouse count
	float stickLookSpeed = 2.5f;   // radians per second at full stick

	void Update(Camera& camera, InputSystem::InputSystem& input, float dt) const;
};

#endif // CAMERA_CONTROLLER_H
