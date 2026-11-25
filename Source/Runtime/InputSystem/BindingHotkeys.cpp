#include "BindingHotkeys.h"
#include "InputSystem.h"

#include <Rendering/Window.h>

#include <SDL3/SDL.h>

namespace InputSystem {

	void BindingHotkeys::ProcessEvent(const SDL_Event& event, Window& window) {
		InputSystem& input = InputSystem::getInstance();

		const bool wasRebinding = input.IsRebinding();
		input.ProcessEvent(event);

		// Save Binding To File
		if (wasRebinding && !input.IsRebinding()) {  // A key was binded, or Esc cancelled binding
			input.SaveBindings(bindingsFile.c_str());
			window.SetTitle(window.title.c_str());
		}

		if (!wasRebinding && event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat) {
			
			const int keyIndex = int(event.key.scancode) - int(SDL_SCANCODE_F1);
			
			if (keyIndex >= 0 && keyIndex < int(actions.size())) {
				const int id = actions[size_t(keyIndex)];
				if (const InputAction* action = input.GetAction(id)) {
					input.BeginRebind(id);
					const std::string prompt = "Press a key for " + action->name + " (Esc cancels)";
					window.SetTitle(prompt.c_str());
				}
			}
		}
	}
}
