#include "BindingHotkeys.h"
#include "InputSystem.h"

#include <Rendering/Window.h>

#include <SDL3/SDL.h>

namespace InputSystem {

	void BindingHotkeys::ProcessEvent(const SDL_Event& e, Window& window) {
		InputSystem& input = InputSystem::getInstance();

		const bool wasRebinding = input.IsRebinding();
		input.ProcessEvent(e);

		if (wasRebinding && !input.IsRebinding()) {  // a key was picked, or Esc cancelled
			input.SaveBindings(bindingsFile_.c_str());
			window.SetTitle(window.title.c_str());
		}

		if (!wasRebinding && e.type == SDL_EVENT_KEY_DOWN && !e.key.repeat) {
			const int f = int(e.key.scancode) - int(SDL_SCANCODE_F1);
			if (f >= 0 && f < int(actions_.size())) {
				const int id = actions_[size_t(f)];
				if (const InputAction* action = input.GetAction(id)) {
					input.BeginRebind(id);
					const std::string prompt = "Press a key for " + action->name + " (Esc cancels)";
					window.SetTitle(prompt.c_str());
				}
			}
		}
	}
}
