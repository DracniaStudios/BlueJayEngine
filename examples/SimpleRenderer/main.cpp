// SimpleRenderer - a minimal 3D renderer example for BlueJay.
//
// A CPU rasterizer draws a spinning cube and pyramid (flat shaded, z-buffered,
// back-face culled) into a framebuffer that SDL3 presents in a window.
//
// main() only configures the app. The window, input and loop live in
// Application/, and the scene lives in SpinningShapes.
//
// Controls (defaults): WASD move, Space/Alt up/down, Shift sprints, mouse looks, F toggles the
// spin, Esc quits. Gamepad (SDL standard layout): left stick moves, right stick looks,
// shoulders go up/down, left stick click sprints, West button toggles the spin, Start quits.
// Rebind keys with F1..F8 (Forward, Backward, Left, Right, Up, Down, Spin, Quit), then press
// the new key; Esc cancels. Bindings are saved to bindings.cfg.

#include <Application/Application.h>
#include <Core/Version.h>
#include <InputSystem/InputSystem.h>
#include <Rendering/Window.h>

#include "SpinningShapes.h"

#include <string>

int main() {
	namespace Inp = InputSystem;

	AppConfig config;
	config.title = std::string("BlueJay SimpleRenderer (engine ") + bluejay::VersionString() + ")";
	config.rebindable = {
		Inp::ACTION_MOVE_FORWARD,
		Inp::ACTION_MOVE_BACKWARD,
		Inp::ACTION_MOVE_LEFT,
		Inp::ACTION_MOVE_RIGHT,
		Inp::ACTION_EDITOR_UP,
		Inp::ACTION_EDITOR_DOWN,
		Inp::ACTION_MOVE_INTERACT,
		Inp::ACTION_UI_PAUSE };

	SpinningShapes game(SCREEN_WIDTH, SCREEN_HEIGHT);
	return Run(config, game);
}
