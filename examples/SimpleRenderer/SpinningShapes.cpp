#include "SpinningShapes.h"

#include <InputSystem/InputSystem.h>

namespace {
	constexpr float kPi = 3.14159265358979f;
}

SpinningShapes::SpinningShapes(int width, int height)
	: proj_(Perspective(60.0f * kPi / 180.0f, float(width) / float(height), 0.1f, 100.0f)) {}

bool SpinningShapes::Update(float dt) {
	namespace Inp = InputSystem;
	Inp::InputSystem& input = Inp::InputSystem::getInstance();

	if (input.IsActionPressed(Inp::ACTION_MOVE_INTERACT)) { paused_ = !paused_; }
	if (!paused_) { angle_ += dt; }
	controller_.Update(camera_, input, dt);

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
