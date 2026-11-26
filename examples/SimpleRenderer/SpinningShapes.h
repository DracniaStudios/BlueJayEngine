#pragma once

#include <Application/Application.h>
#include <Rendering/Camera.h>
#include <Rendering/CameraController.h>

// The demo scene: a spinning cube and pyramid with a row of cubes behind them.
class SpinningShapes : public Game {
public:
	SpinningShapes(int width, int height);

	bool Update(float dt) override;
	void Render(Renderer& renderer) override;
	void EnableFlyThrough() override;

private:
	const Mesh cube_ = MakeCube();
	const Mesh pyramid_ = MakePyramid();
	const Matrix4x4 proj_;
	const Vector3 lightWorld_ = Normalize({ 0.4f, 0.8f, 0.6f });  // toward the light, world space
	Camera camera_;  // starts at +Z looking down -Z
	CameraController controller_;
	float angle_ = 0.0f;
	bool paused_ = false;
	bool flyThrough_ = false;  // camera follows a fixed path, move/look input is ignored
	float flyTime_ = 0.0f;
};
