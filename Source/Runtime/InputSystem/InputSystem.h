#pragma once
#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

// InputSystem - action-based input on top of SDL3.
//
// Keys are SDL scancodes (physical keys, independent of keyboard layout).
// Gamepads go through SDL's gamepad mapping layer, so every controller exposes
// the same standard layout. Gamepad buttons are SDL_GamepadButton values, plus
// two pseudo-buttons for the analog triggers (SDL reports triggers as axes).
//
// SDL types stay out of this header (see cmake/CheckPublicHeaders.cmake); they
// are forward declared only and every SDL call lives in InputSystem.cpp.
//
// Per frame:   while (SDL_PollEvent(&e)) InputSystem::getInstance().ProcessEvent(e);
//              InputSystem::getInstance().Update();

#include <array>
#include <cstdint>
#include <string>
#include <vector>

union SDL_Event;
struct SDL_Gamepad;

namespace InputSystem {

	constexpr int KEY_NULL = 0;                 // SDL_SCANCODE_UNKNOWN: no key bound
	constexpr int kMaxKeys = 512;               // >= SDL_SCANCODE_COUNT (checked in the .cpp)
	constexpr int GAMEPAD_BUTTON_NONE = -1;     // no gamepad binding
	constexpr int GAMEPAD_TRIGGER_LEFT = 100;   // analog trigger used as a button
	constexpr int GAMEPAD_TRIGGER_RIGHT = 101;
	constexpr int kMaxButtons = 128;

	enum ActionType : uint8_t {
		NO_ACTION = 0,

		// Move Actions
		ACTION_MOVE_FORWARD,
		ACTION_MOVE_BACKWARD,
		ACTION_MOVE_RIGHT,
		ACTION_MOVE_LEFT,

		ACTION_MOVE_CROUCH,
		ACTION_MOVE_SPRINT,
		ACTION_MOVE_INTERACT,
		ACTION_MOVE_JUMP,

		// Artifact Actions
		ACTION_USE_ARTIFACT,
		ACTION_USE_ARTIFACT_RIGHT,
		ACTION_USE_ARTIFACT_LEFT,

		// Item Actions
		ACTION_USE_ITEM,
		ACTION_USE_ITEM2,
		ACTION_USE_FLASHLIGHT,

		// Editor Actions
		ACTION_EDITOR_UP,
		ACTION_EDITOR_DOWN,

		// Menu Actions
		ACTION_UI_BACK, // Backs out of the overlay on top
		ACTION_UI_PAUSE,

		MAX_ACTION
	};

	struct InputAction {
		std::string name = "";
		bool isEnabled = true;
		int id = NO_ACTION;
		int key = KEY_NULL;                 // SDL scancode
		int button = GAMEPAD_BUTTON_NONE;   // SDL_GamepadButton or GAMEPAD_TRIGGER_*
	};

	class InputSystem
	{
	private:
		InputSystem();

		std::vector<int> pressedKeys;  // Keys pressed this frame, in press order
		std::vector<int> releasedKeys; // Keys released this frame
		std::vector<int> heldKeys;     // Keys currently held down

		std::vector<int> pressedButtons;  // Gamepad buttons pressed this frame
		std::vector<int> releasedButtons; // Gamepad buttons released this frame
		std::array<bool, kMaxButtons> buttonsDown{};
		std::array<bool, kMaxButtons> buttonsPrev{};

		// Filled by ProcessEvent, published to the vectors above by Update().
		std::vector<int> pendingPressed;
		std::vector<int> pendingReleased;
		float pendingMouseDx = 0, pendingMouseDy = 0;
		float mouseDx = 0, mouseDy = 0;

		SDL_Gamepad* gamepad = nullptr;

		bool rebinding = false;
		bool rebindGamepad = false;
		int rebindAction = NO_ACTION;

		bool KeyDown(int key) const;
		bool ButtonDown(int button) const;
		static bool Contains(const std::vector<int>& v, int value);
	public:
		InputSystem(const InputSystem&) = delete;
		InputSystem& operator=(const InputSystem&) = delete;
		InputSystem(InputSystem&&) = delete;
		InputSystem& operator=(InputSystem&&) = delete;

		static InputSystem& getInstance() {
			static InputSystem instance;
			return instance;
		}
		std::vector<InputAction> inputActions; // Indexed by ActionType

		// Call after SDL_Init, and before SDL_Quit (the singleton outlives SDL).
		void Init();
		void Shutdown();

		// Feed every SDL event here, then call Update() once per frame before the scene update.
		void ProcessEvent(const SDL_Event& e);
		void Update();

		// Action Functions
		bool IsActionDown(int action);
		bool IsActionPressed(int action);
		bool IsActionReleased(int action);

		InputAction* GetAction(int action);

		void SetDefaultActions(void);
		void ResetToDefaults(void);
		void SetAction(int action, int key, int button);
		void CreateAction(int action, const std::string& name, int key, int button);

		// Key Functions
		const std::vector<int>& GetPressedKeys() const { return pressedKeys; } // Keys pressed this frame
		const std::vector<int>& GetHeldKeys() const { return heldKeys; }       // Keys currently held down
		int GetLastPressedKey() const { return pressedKeys.empty() ? KEY_NULL : pressedKeys.back(); } // KEY_NULL if none this frame
		int GetLastPressedButton() const { return pressedButtons.empty() ? GAMEPAD_BUTTON_NONE : pressedButtons.back(); }

		// Analog input. Sticks have a radial deadzone; x is right-positive, y is up-positive.
		void GetStick(bool rightStick, float& x, float& y) const;
		void GetMouseDelta(float& dx, float& dy) const { dx = mouseDx; dy = mouseDy; } // counts this frame
		bool HasGamepad() const { return gamepad != nullptr; }

		// Rebinding: the next key (or gamepad button) pressed becomes the action's binding.
		// Esc cancels. Several actions may share a key on purpose (e.g. Jump and Editor Up).
		void BeginRebind(int action, bool forGamepad = false);
		void CancelRebind() { rebinding = false; }
		bool IsRebinding() const { return rebinding; }
		int GetRebindAction() const { return rebindAction; }

		// Plain text, one line per action: "Move Forward = W | dpup".
		bool SaveBindings(const char* path) const;
		bool LoadBindings(const char* path);

		static std::string KeyName(int key);
		static std::string ButtonName(int button);
	};
}
#endif
