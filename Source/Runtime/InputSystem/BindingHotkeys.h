#pragma once
#ifndef BINDING_HOTKEYS_H
#define BINDING_HOTKEYS_H

// F1, F2, ... start rebinding the listed actions, in order. The window title
// shows the prompt, and bindings are saved once a key is picked or Esc cancels.

#include <string>
#include <utility>
#include <vector>

union SDL_Event;
class Window;

namespace InputSystem {

	class BindingHotkeys {
	public:
		BindingHotkeys(std::vector<int> actions, std::string bindingsFile)
			: actions(std::move(actions)), bindingsFile(std::move(bindingsFile)) {}

		// Use instead of InputSystem::ProcessEvent; it forwards the event.
		void ProcessEvent(const SDL_Event& e, Window& window);

	private:
		std::vector<int> actions;
		std::string bindingsFile;
	};
}

#endif // BINDING_HOTKEYS_H
