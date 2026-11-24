#include "InputSystem.h"

#include <SDL3/SDL.h>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>

namespace InputSystem {

	static_assert(kMaxKeys >= SDL_SCANCODE_COUNT, "kMaxKeys too small for SDL scancodes");
	static_assert(kMaxButtons > GAMEPAD_TRIGGER_RIGHT, "kMaxButtons too small for trigger pseudo-buttons");
	static_assert(kMaxButtons >= SDL_GAMEPAD_BUTTON_COUNT, "kMaxButtons too small for SDL gamepad buttons");

	namespace {
		constexpr float kStickDeadzone = 0.15f;
		constexpr float kTriggerThreshold = 0.5f;

		std::string Trim(const std::string& s) {
			const size_t b = s.find_first_not_of(" \t\r\n");
			if (b == std::string::npos) return {};
			const size_t e = s.find_last_not_of(" \t\r\n");
			return s.substr(b, e - b + 1);
		}

		float AxisValue(SDL_Gamepad* pad, SDL_GamepadAxis axis) {
			return std::clamp(float(SDL_GetGamepadAxis(pad, axis)) / 32767.0f, -1.0f, 1.0f);
		}
	}

	InputSystem::InputSystem() {
		inputActions.resize(MAX_ACTION); // One slot per ActionType, indexed by action id

		// Defaults first; call LoadBindings() to override them from a file.
		SetDefaultActions();
	}

	void InputSystem::Init() {
		// ADDED events for controllers that are already plugged in arrive through ProcessEvent.
		SDL_InitSubSystem(SDL_INIT_GAMEPAD);
	}

	void InputSystem::Shutdown() {
		if (gamepad) { SDL_CloseGamepad(gamepad); gamepad = nullptr; }
		SDL_QuitSubSystem(SDL_INIT_GAMEPAD);
	}

	bool InputSystem::Contains(const std::vector<int>& v, int value) {
		return std::find(v.begin(), v.end(), value) != v.end();
	}

	bool InputSystem::KeyDown(int key) const {
		if (key <= KEY_NULL || key >= kMaxKeys) { return false; }
		int count = 0;
		const bool* state = SDL_GetKeyboardState(&count);
		return key < count && state[key];
	}

	bool InputSystem::ButtonDown(int button) const {
		return button >= 0 && button < kMaxButtons && buttonsDown[size_t(button)];
	}

	void InputSystem::ProcessEvent(const SDL_Event& e) {
		switch (e.type) {
		case SDL_EVENT_GAMEPAD_ADDED:
			if (!gamepad) { gamepad = SDL_OpenGamepad(e.gdevice.which); }
			break;

		case SDL_EVENT_GAMEPAD_REMOVED:
			if (gamepad && SDL_GetGamepadID(gamepad) == e.gdevice.which) {
				SDL_CloseGamepad(gamepad);
				gamepad = nullptr;
			}
			break;

		case SDL_EVENT_KEY_DOWN: {
			if (e.key.repeat) { break; }
			const int key = int(e.key.scancode);

			if (rebinding) {
				// The press is consumed: it must not also trigger an action.
				if (key == int(SDL_SCANCODE_ESCAPE)) {
					rebinding = false;
				} else if (!rebindGamepad && key > KEY_NULL && key < kMaxKeys) {
					if (auto* input = GetAction(rebindAction)) { input->key = key; }
					rebinding = false;
				}
				break;
			}
			if (key > KEY_NULL && key < kMaxKeys) { pendingPressed.push_back(key); }
			break;
		}

		case SDL_EVENT_KEY_UP: {
			const int key = int(e.key.scancode);
			if (key > KEY_NULL && key < kMaxKeys) { pendingReleased.push_back(key); }
			break;
		}

		case SDL_EVENT_MOUSE_MOTION:
			pendingMouseDx += e.motion.xrel;
			pendingMouseDy += e.motion.yrel;
			break;

		default:
			break;
		}
	}

	void InputSystem::Update() {
		// Keys: publish what the events collected since the last frame.
		pressedKeys.swap(pendingPressed);
		releasedKeys.swap(pendingReleased);
		pendingPressed.clear();
		pendingReleased.clear();

		mouseDx = pendingMouseDx;
		mouseDy = pendingMouseDy;
		pendingMouseDx = pendingMouseDy = 0;

		heldKeys.clear();
		int count = 0;
		const bool* state = SDL_GetKeyboardState(&count);
		for (int key = 1; key < std::min(count, kMaxKeys); ++key) {
			if (state[key]) { heldKeys.push_back(key); }
		}

		// Gamepad buttons: poll and diff against last frame.
		buttonsPrev = buttonsDown;
		buttonsDown.fill(false);
		if (gamepad) {
			for (int b = 0; b < SDL_GAMEPAD_BUTTON_COUNT; ++b) {
				buttonsDown[size_t(b)] = SDL_GetGamepadButton(gamepad, SDL_GamepadButton(b));
			}
			buttonsDown[GAMEPAD_TRIGGER_LEFT] = AxisValue(gamepad, SDL_GAMEPAD_AXIS_LEFT_TRIGGER) > kTriggerThreshold;
			buttonsDown[GAMEPAD_TRIGGER_RIGHT] = AxisValue(gamepad, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) > kTriggerThreshold;
		}

		pressedButtons.clear();
		releasedButtons.clear();
		for (int b = 0; b < kMaxButtons; ++b) {
			if (buttonsDown[size_t(b)] && !buttonsPrev[size_t(b)]) { pressedButtons.push_back(b); }
			if (!buttonsDown[size_t(b)] && buttonsPrev[size_t(b)]) { releasedButtons.push_back(b); }
		}

		if (rebinding && rebindGamepad && !pressedButtons.empty()) {
			if (auto* input = GetAction(rebindAction)) { input->button = pressedButtons.front(); }
			rebinding = false;
		}
	}

	void InputSystem::GetStick(bool rightStick, float& x, float& y) const {
		x = y = 0.0f;
		if (!gamepad) { return; }

		const float rx = AxisValue(gamepad, rightStick ? SDL_GAMEPAD_AXIS_RIGHTX : SDL_GAMEPAD_AXIS_LEFTX);
		const float ry = -AxisValue(gamepad, rightStick ? SDL_GAMEPAD_AXIS_RIGHTY : SDL_GAMEPAD_AXIS_LEFTY); // SDL: up is negative

		const float mag = std::sqrt(rx * rx + ry * ry);
		if (mag < kStickDeadzone) { return; }

		const float scale = std::min(1.0f, (mag - kStickDeadzone) / (1.0f - kStickDeadzone)) / mag;
		x = rx * scale;
		y = ry * scale;
	}

	InputAction* InputSystem::GetAction(int action) {
		if (action <= NO_ACTION || action >= MAX_ACTION) { return nullptr; }
		return &inputActions[action];
	}

	bool InputSystem::IsActionDown(int action) {
		const auto input = GetAction(action);
		if (!input || !input->isEnabled) { return false; }
		return KeyDown(input->key) || ButtonDown(input->button);
	}

	bool InputSystem::IsActionPressed(int action) {
		const auto input = GetAction(action);
		if (!input || !input->isEnabled) { return false; }
		return (input->key != KEY_NULL && Contains(pressedKeys, input->key))
			|| (input->button >= 0 && Contains(pressedButtons, input->button));
	}

	bool InputSystem::IsActionReleased(int action) {
		const auto input = GetAction(action);
		if (!input || !input->isEnabled) { return false; }
		return (input->key != KEY_NULL && Contains(releasedKeys, input->key))
			|| (input->button >= 0 && Contains(releasedButtons, input->button));
	}

	void InputSystem::SetAction(int action, int key, int button) {
		const auto input = GetAction(action);
		if (!input) { std::cout << "Unknown Action: " << action << "\n"; return; }
		input->key = key;
		input->button = button;
	}

	void InputSystem::CreateAction(int action, const std::string& name, int key, int button) {
		const auto input = GetAction(action);
		if (!input) { std::cout << "Invalid Action: " << action << "\n"; return; }
		if (input->id != NO_ACTION) { std::cout << "Existing Action: " << input->name << "\n"; return; }

		input->id = action;
		input->name = name;
		input->key = key;
		input->button = button;
	}

	void InputSystem::ResetToDefaults() {
		for (auto& input : inputActions) { input = InputAction{}; }
		SetDefaultActions();
	}

	void InputSystem::SetDefaultActions() {

		// Gamepad buttons use SDL's standard layout: SOUTH/EAST/WEST/NORTH are the
		// face buttons (A/B/X/Y on Xbox), BACK/START the two middle buttons.
		// Analog stick movement is read with GetStick().

		// Move Action
		CreateAction(ACTION_MOVE_FORWARD, "Move Forward", SDL_SCANCODE_W, SDL_GAMEPAD_BUTTON_DPAD_UP);
		CreateAction(ACTION_MOVE_BACKWARD, "Move Backward", SDL_SCANCODE_S, SDL_GAMEPAD_BUTTON_DPAD_DOWN);
		CreateAction(ACTION_MOVE_RIGHT, "Move Right", SDL_SCANCODE_D, SDL_GAMEPAD_BUTTON_DPAD_RIGHT);
		CreateAction(ACTION_MOVE_LEFT, "Move Left", SDL_SCANCODE_A, SDL_GAMEPAD_BUTTON_DPAD_LEFT);

		CreateAction(ACTION_MOVE_CROUCH, "Crouch", SDL_SCANCODE_LCTRL, SDL_GAMEPAD_BUTTON_EAST);
		CreateAction(ACTION_MOVE_SPRINT, "Sprint", SDL_SCANCODE_LSHIFT, SDL_GAMEPAD_BUTTON_LEFT_STICK);
		CreateAction(ACTION_MOVE_INTERACT, "Interact", SDL_SCANCODE_F, SDL_GAMEPAD_BUTTON_WEST);
		CreateAction(ACTION_MOVE_JUMP, "Jump", SDL_SCANCODE_SPACE, SDL_GAMEPAD_BUTTON_SOUTH);

		CreateAction(ACTION_USE_ARTIFACT, "Artifact_Interact", SDL_SCANCODE_E, SDL_GAMEPAD_BUTTON_BACK);
		CreateAction(ACTION_USE_ARTIFACT_RIGHT, "Artifact_Right", SDL_SCANCODE_R, SDL_GAMEPAD_BUTTON_RIGHT_STICK);
		CreateAction(ACTION_USE_ARTIFACT_LEFT, "Artifact_Left", SDL_SCANCODE_Q, SDL_GAMEPAD_BUTTON_GUIDE);

		// Item Actions (the triggers are analog axes in SDL, exposed as pseudo-buttons)
		CreateAction(ACTION_USE_ITEM, "Item_Left", SDL_SCANCODE_T, GAMEPAD_TRIGGER_LEFT);
		CreateAction(ACTION_USE_ITEM2, "Item_Right", SDL_SCANCODE_G, GAMEPAD_TRIGGER_RIGHT);
		CreateAction(ACTION_USE_FLASHLIGHT, "Flashlight", SDL_SCANCODE_L, SDL_GAMEPAD_BUTTON_NORTH);

		// Editor Actions
		// LCTRL is also the modifier every Ctrl+ editor hotkey reads directly
		// (panel toggles, undo, duplicate - see WorldEditor::UpdateHotkeys); binding it
		// here too meant holding Ctrl for any of those also flew the camera upward for
		// as long as it was held.
		CreateAction(ACTION_EDITOR_UP, "Editor_Up", SDL_SCANCODE_SPACE, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER);
		CreateAction(ACTION_EDITOR_DOWN, "Editor_Down", SDL_SCANCODE_LALT, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER);

		// UI Action
		CreateAction(ACTION_UI_BACK, "Back", SDL_SCANCODE_TAB, SDL_GAMEPAD_BUTTON_BACK);
		CreateAction(ACTION_UI_PAUSE, "Pause", SDL_SCANCODE_ESCAPE, SDL_GAMEPAD_BUTTON_START);

	}

	void InputSystem::BeginRebind(int action, bool forGamepad) {
		if (!GetAction(action)) { return; }
		rebinding = true;
		rebindGamepad = forGamepad;
		rebindAction = action;
	}

	std::string InputSystem::KeyName(int key) {
		if (key <= KEY_NULL || key >= kMaxKeys) { return "none"; }
		const char* name = SDL_GetScancodeName(SDL_Scancode(key));
		return (name && *name) ? name : "none";
	}

	std::string InputSystem::ButtonName(int button) {
		if (button == GAMEPAD_TRIGGER_LEFT) { return "lefttrigger"; }
		if (button == GAMEPAD_TRIGGER_RIGHT) { return "righttrigger"; }
		if (button < 0 || button >= SDL_GAMEPAD_BUTTON_COUNT) { return "none"; }
		const char* name = SDL_GetGamepadStringForButton(SDL_GamepadButton(button));
		return name ? name : "none";
	}

	bool InputSystem::SaveBindings(const char* path) const {
		std::ofstream file(path, std::ios::trunc);
		if (!file) { return false; }

		file << "# BlueJay input bindings:  Action = Key | GamepadButton\n";
		file << "# Key names come from SDL_GetScancodeName, button names from SDL's gamepad API.\n";
		for (const InputAction& input : inputActions) {
			if (input.id == NO_ACTION) { continue; }
			file << input.name << " = " << KeyName(input.key) << " | " << ButtonName(input.button) << "\n";
		}
		return bool(file);
	}

	bool InputSystem::LoadBindings(const char* path) {
		std::ifstream file(path);
		if (!file) { return false; }

		std::string line;
		while (std::getline(file, line)) {
			line = Trim(line);
			if (line.empty() || line[0] == '#') { continue; }

			// The action name never contains '=', but a key name can ("="), so split at the first one.
			const size_t eq = line.find('=');
			if (eq == std::string::npos) { continue; }
			const std::string name = Trim(line.substr(0, eq));
			const std::string rest = line.substr(eq + 1);

			// Likewise split key | button at the last '|'.
			const size_t bar = rest.rfind('|');
			if (bar == std::string::npos) { continue; }
			const std::string keyText = Trim(rest.substr(0, bar));
			const std::string buttonText = Trim(rest.substr(bar + 1));

			InputAction* target = nullptr;
			for (InputAction& input : inputActions) {
				if (input.id != NO_ACTION && input.name == name) { target = &input; }
			}
			if (!target) { continue; } // unknown action: skip, so older/newer files keep loading

			int key = KEY_NULL;
			if (keyText != "none") {
				const SDL_Scancode sc = SDL_GetScancodeFromName(keyText.c_str());
				if (sc != SDL_SCANCODE_UNKNOWN) { key = int(sc); }
				else { key = target->key; } // unrecognised name: keep the current binding
			}

			int button = GAMEPAD_BUTTON_NONE;
			if (buttonText == "lefttrigger") { button = GAMEPAD_TRIGGER_LEFT; }
			else if (buttonText == "righttrigger") { button = GAMEPAD_TRIGGER_RIGHT; }
			else if (buttonText != "none") {
				const SDL_GamepadButton b = SDL_GetGamepadButtonFromString(buttonText.c_str());
				button = (b == SDL_GAMEPAD_BUTTON_INVALID) ? target->button : int(b);
			}

			target->key = key;
			target->button = button;
		}
		return true;
	}
}
