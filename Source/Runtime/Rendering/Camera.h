#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <Core/Core.h>

class Camera {
public:
	Vector3 position{ 0, 0, 7 };
	float yaw = 0.0f;    // radians, 0 = looking down -Z
	float pitch = 0.0f;  // radians, positive = looking up

	Vector3 Forward() const {
		return { -std::sin(yaw) * std::cos(pitch),
		          std::sin(pitch),
		         -std::cos(yaw) * std::cos(pitch) };
	}
	Vector3 Right() const { return { std::cos(yaw), 0, -std::sin(yaw) }; }

	void Look(float dYaw, float dPitch) {
		yaw += dYaw;
		pitch = std::clamp(pitch + dPitch, -1.55f, 1.55f);  // stay just under +-90 degrees
	}

	// Inverse of the camera's world transform T(pos) * Ry(yaw) * Rx(pitch).
	Matrix4x4 View() const {
		return RotationX(-pitch) * RotationY(-yaw) * Translation(position * -1.0f);
	}
};

#endif // CAMERA_H
