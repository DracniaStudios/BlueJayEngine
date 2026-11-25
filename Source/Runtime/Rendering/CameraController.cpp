#include "CameraController.h"

#include <InputSystem/InputSystem.h>

void CameraController::Update(Camera& camera, InputSystem::InputSystem& input, float dt) const {
	namespace Inp = InputSystem;

	// Look: mouse deltas are per frame, stick look is a rate so it is scaled by dt.
	float mouseDx, mouseDy, lookStickX, lookStickY, moveStickX, moveStickY;
	input.GetMouseDelta(mouseDx, mouseDy);
	input.GetStick(true, lookStickX, lookStickY);
	input.GetStick(false, moveStickX, moveStickY);
	camera.Look(-mouseDx * mouseSens - lookStickX * stickLookSpeed * dt,
	            -mouseDy * mouseSens + lookStickY * stickLookSpeed * dt);

	auto axis = [&](int positive, int negative) {
		return float(input.IsActionDown(positive)) - float(input.IsActionDown(negative));
	};
	float moveX = axis(Inp::ACTION_MOVE_RIGHT, Inp::ACTION_MOVE_LEFT) + moveStickX;
	float moveY = axis(Inp::ACTION_EDITOR_UP, Inp::ACTION_EDITOR_DOWN);
	float moveZ = axis(Inp::ACTION_MOVE_FORWARD, Inp::ACTION_MOVE_BACKWARD) + moveStickY;
	const float moveLen = std::sqrt(moveX * moveX + moveY * moveY + moveZ * moveZ);
	if (moveLen > 1.0f) { moveX /= moveLen; moveY /= moveLen; moveZ /= moveLen; }  // no faster diagonally

	const float speed = moveSpeed * (input.IsActionDown(Inp::ACTION_MOVE_SPRINT) ? sprintMultiplier : 1.0f);
	const Vector3 move = camera.Forward() * moveZ + camera.Right() * moveX + Vector3{ 0, moveY, 0 };
	camera.position = camera.position + move * (speed * dt);
}
