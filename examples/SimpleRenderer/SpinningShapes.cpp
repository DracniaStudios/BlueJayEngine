#include "SpinningShapes.h"

#include <InputSystem/InputSystem.h>

#include <algorithm>
#include <cmath>
#include <iterator>

namespace {
	constexpr float kPi = 3.14159265358979f;

	// Fly-through: a loop around the scene. Yaw is kept continuous (no wrap) so each leg
	// turns the short way. The last waypoint repeats the first one.
	struct Waypoint { Vector3 position; float yaw, pitch; };
	constexpr Waypoint kPath[] = {
		{ {   0.0f, 0.0f,  7.0f },  0.0f,        0.0f  },
		{ {  -8.0f, 1.5f,  4.0f }, -0.6f,       -0.15f },
		{ { -12.0f, 1.0f, -1.0f }, -1.2f,        0.0f  },
		{ {   0.0f, 4.0f, -9.0f }, -kPi,        -0.4f  },
		{ {  12.0f, 1.0f, -1.0f }, -4.9f,        0.0f  },
		{ {   8.0f, 1.5f,  4.0f }, -5.68f,      -0.15f },
		{ {   0.0f, 0.0f,  7.0f }, -2.0f * kPi,  0.0f  },
	};
	constexpr int kLegs = int(std::size(kPath)) - 1;
	constexpr float kLegSeconds = 5.0f;  // 6 legs = one 30 s loop

	void FollowPath(Camera& camera, float time) {
		const float t = std::fmod(time, kLegs * kLegSeconds) / kLegSeconds;
		const int leg = std::min(int(t), kLegs - 1);
		float s = t - float(leg);
		s = s * s * (3.0f - 2.0f * s);  // smoothstep: ease in and out at each waypoint

		const Waypoint& a = kPath[leg];
		const Waypoint& b = kPath[leg + 1];
		camera.position = a.position + (b.position - a.position) * s;
		camera.yaw = a.yaw + (b.yaw - a.yaw) * s;
		camera.pitch = a.pitch + (b.pitch - a.pitch) * s;
	}
}

SpinningShapes::SpinningShapes(int width, int height)
	: proj_(Perspective(60.0f * kPi / 180.0f, float(width) / float(height), 0.1f, 100.0f)) {}

bool SpinningShapes::Update(float dt) {
	namespace Inp = InputSystem;
	Inp::InputSystem& input = Inp::InputSystem::getInstance();

	if (input.IsActionPressed(Inp::ACTION_MOVE_INTERACT)) { paused_ = !paused_; }
	if (!paused_) { angle_ += dt; }
	if (flyThrough_) {
		flyTime_ += dt;
		FollowPath(camera_, flyTime_);
	}
	else {
		controller_.Update(camera_, input, dt);
	}

	return !input.IsActionPressed(Inp::ACTION_UI_PAUSE);
}

void SpinningShapes::Render(Renderer& renderer) {
	const Matrix4x4 view = camera_.View();
	const Vector3 lightDir = camera_.ToViewSpace(lightWorld_);  // light stays fixed in the world

	renderer.Clear(0xFF14181F);
	renderer.DrawMesh(cube_, Translation({ -2.0f, 0, 0 }) * RotationY(angle_) * RotationX(angle_ * 0.6f),
	                  view, proj_, lightDir);
	renderer.DrawMesh(pyramid_, Translation({ 2.2f, 0, 0 }) * RotationY(-angle_ * 1.3f), view, proj_,
	                  lightDir);

	for (int i = 0; i < 10; i++) {
		renderer.DrawMesh(cube_, Translation({ i * 3.0f - 10, 0, -5 }) * RotationY(-angle_ * 1.3f), view, proj_,
		                  lightDir);
	}
}

void SpinningShapes::EnableFlyThrough() {
	flyThrough_ = true;
	flyTime_ = 0.0f;
}
